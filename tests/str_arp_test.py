#!/usr/bin/env python3
"""Run the CTL arpeggiator and fret-event handlers with host hardware stubs."""
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
CTL = ROOT / "software/firmwareCtl"


def function(source, signature):
    start = source.index(signature)
    opening = source.index("{", start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


main = (CTL / "firmwareCtl.ino").read_text()
arp = (CTL / "09_op02_StrArp.ino").read_text()
hid = (CTL / "04_hid.ino").read_text()
midi = (CTL / "06_midi.ino").read_text()
# Use the actual firmware state declarations and implementation, not a second model.
state = main.split("//string arpeggiator/sequencer\n", 1)[1].split("//generic Sequenzer", 1)[0]
colors = "\n".join(re.findall(r"^  const float strArp_.*;", main, re.M))
prototypes = "\n".join(signature + ";" for signature in re.findall(
    r"^([A-Za-z_][^\n{}]*\([^;\n]*\))\s*\{", arp, re.M))

stubs = r'''
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using byte = unsigned char;
const int nStrings=6, nFrets=21, frtSplit=22;
byte strPrs[nStrings];
bool strHold[nStrings], fbrdMode, mtOut;
int lastNZStrPrs[nStrings];
int tuning[nStrings]={64,59,55,50,45,40};
byte frtb_sensMode, strSeq_act;
bool strArp_modeSel=true;
long mClock, lastTick=-1;
int dispEncMode, opMode, dispEncFnc[8], nDispEncFnc[8];
const int genSq_opMode=2, genSq_nInst=3, genSq_nPttn=6, genSq_strEncFnc_chn=0;
byte genSq_chn[3][6][6][1], genSq_actPttn[3];
bool genSq_muteCh[3][6];
float trgtC[nStrings][25][3];
int envelopes[nStrings], kicks[nStrings], manual[nStrings];
struct Event { long tick; int note, velocity, channel; };
std::vector<Event> events;
struct MidiStub {
  void sendNoteOn(int note, int velocity, int channel) {
    events.push_back({mClock,note,velocity,channel});
  }
  void sendNoteOff(int note, int velocity, int channel) {
    events.push_back({mClock,note,velocity,channel});
  }
} usbMIDI;
void delay(int) {}
void sndTrigEnv(byte s, float) { envelopes[s]++; }
void kick(byte s) { kicks[s]++; }
void sndManualEnv(byte s, bool state) { manual[s]=state; }
void sndStrPrs(byte, int, int) {}
void genSq_editSteps(int) {}
void genSq_sndStpOffNMW(int, int, byte) {}
template<class T> T constrain(T value, int low, int high) {
  return std::max(T(low), std::min(value, T(high)));
}
#define DISPLAY_STUB(name) template<class... T> void name(T...) {}
DISPLAY_STUB(disp_Color)
DISPLAY_STUB(disp_RBox)
DISPLAY_STUB(disp_RFrm)
DISPLAY_STUB(disp_Str)
DISPLAY_STUB(disp_Int)
'''

tests = r'''
void reset() {
  for(int s=0;s<nStrings;s++)sndMidiNotePress(s,0,1);
  events.clear();
  for(int s=0;s<nStrings;s++) {
    strPrs[s]=0; strHold[s]=false; lastNZStrPrs[s]=0;
    strArp_pressOrder[s]=0;
    envelopes[s]=kicks[s]=manual[s]=0;
    genSq_muteCh[0][s]=false;
  }
  strArp_pressOrderNext=1;
  strArp_patterns=StrArpPatternBank{};
  for(int s=0;s<nStrings;s++) {
    strArp_steps[s]=strArp_repeat[s]=1;
    strArp_tmDv[s]=6; strArp_chn[s]=s+1;
  }
  for(auto &playback:strArp_playback)playback=StrArpPlaybackState{};
  strArp_modeSel=true; strArp_act=1; strSeq_act=0;
  fbrdMode=mtOut=false; frtb_sensMode=0; mClock=0; lastTick=-1;
}
void press(int s, int fret, bool held=false) {
  strHold[s]=held; strPrs[s]=fret;
  handleFretEventForAudioMidiKickSeq(s,fret,0);
}
void release(int s) {
  strPrs[s]=0;
  handleFretEventForAudioMidiKickSeq(s,0,0);
}
void tick(long clock) {
  while(lastTick<clock) { mClock=++lastTick; strArp_updClck(); }
}
int count(int note, int channel, bool on) {
  return std::count_if(events.begin(), events.end(), [&](const Event &e) {
    return e.note==note && e.channel==channel && (e.velocity>0)==on;
  });
}
void sequence(int held, std::initializer_list<int> expected) {
  const auto &playback=strArp_playback[held];
  assert(playback.seqLen==expected.size());
  assert(std::equal(expected.begin(),expected.end(),playback.seq));
}
int note(int s, int fret) { return tuning[s]+fret+12; }
int main() {
  // A released held string repeats its last fret as MIDI only.
  reset(); press(0,4,true); release(0); tick(0); tick(6);
  sequence(0,{}); sequence(1,{0});
  assert(count(note(0,4),1,true)==2);
  assert(count(note(0,4),1,false)==1);
  assert(envelopes[0]==0 && kicks[0]==0);

  // Multiple held strings preserve press order after release and update pitch.
  reset(); press(3,7,true); press(1,5,true); release(3); release(1);
  tick(0); tick(6); tick(12);
  sequence(1,{3,1});
  assert(count(note(3,7),4,true)==2);
  assert(count(note(1,5),2,true)==1);
  press(1,8,true); release(1); tick(18);
  assert(count(note(1,8),2,true)==1);
  assert(count(note(1,5),2,false)==1);
  for(int s=0;s<nStrings;s++)assert(envelopes[s]==0 && kicks[s]==0);

  // Independent durations, repeats and Note Off ownership, even with equal order.
  reset(); press(0,4); press(1,5,true); release(1);
  strArp_order[0]=strArp_order[1]=1;
  strArp_tmDv[0]=4; strArp_tmDv[1]=6; strArp_repeat[1]=3;
  for(int t=0;t<=6;t++)tick(t);
  sequence(0,{0}); sequence(1,{1});
  assert(count(note(0,4),1,true)==2 && count(note(0,4),1,false)==1);
  assert(count(note(1,5),2,true)==4 && count(note(1,5),2,false)==3);
  assert(envelopes[0]==2 && kicks[0]==2);
  assert(envelopes[1]==0 && kicks[1]==0);
  assert(strArp_clk[0]==0 && strArp_clk[1]==0);

  // A normal step change must not end the longer held note early.
  reset(); press(0,4); press(1,5,true); press(2,6); release(1);
  strArp_tmDv[0]=2; strArp_tmDv[1]=6;
  tick(0); tick(1); tick(2);
  sequence(0,{0,2}); sequence(1,{1});
  assert(count(note(1,5),2,false)==0);
  assert(count(note(0,4),1,false)==1 && count(note(2,6),3,true)==1);

  // Held order groups play together, but never trigger the actuators.
  reset(); press(1,5,true); press(2,6,true); release(1); release(2);
  strArp_order[1]=strArp_order[2]=2; strArp_repeat[1]=3;
  tick(0); tick(2); tick(4);
  sequence(1,{1});
  assert(count(note(1,5),2,true)==3 && count(note(2,6),3,true)==3);
  assert(envelopes[1]==0 && envelopes[2]==0 && kicks[1]==0 && kicks[2]==0);

  // Both sequences obey each direction and step counts, including when empty.
  const std::vector<std::vector<int>> expected={{0,1,1},{1,1,0},{0,1,1,1,1,0},{1,1,0,0,1,1}};
  for(int direction=0;direction<4;direction++) {
    reset(); strArp_strPrsFnc=direction; tick(0);
    press(0,4,true); press(1,5,true); press(2,6); press(3,7);
    release(0); release(1); strArp_steps[1]=strArp_steps[3]=2;
    strArp_mkArp();
    for(int held=0;held<2;held++) {
      const auto &playback=strArp_playback[held];
      assert(playback.seqLen==expected[direction].size());
      for(int i=0;i<playback.seqLen;i++)assert(playback.seq[i]==expected[direction][i]+(held?0:2));
    }
  }

  // Adding the held arpeggio aligns it to the existing tick phase.
  reset(); press(0,4); tick(0); tick(1); tick(2);
  press(1,5,true); release(1); tick(3); tick(4); tick(5);
  assert(count(note(1,5),2,true)==0);
  tick(6); assert(count(note(1,5),2,true)==1);
  assert(count(note(0,4),1,true)==2);

  // Mute, unhold, output mute and no saved fret all silence held notes.
  reset(); press(0,4,true); release(0); tick(0);
  strArp_muteCh[0]=true; tick(1);
  assert(count(note(0,4),1,false)==1);
  strArp_muteCh[0]=false; tick(6);
  strHold[0]=false; tick(7);
  assert(count(note(0,4),1,false)==2);
  sequence(1,{}); assert(strArp_pressOrder[0]==0);
  reset(); press(0,4,true); release(0); tick(0); mtOut=true; tick(1);
  assert(count(note(0,4),1,false)==1);
  reset(); strHold[0]=true; tick(0); sequence(1,{}); assert(events.empty());

  // Zero-step held strings stay MIDI-only even if the sequencer track is muted.
  reset(); strArp_steps[0]=0; genSq_muteCh[0][0]=true;
  press(0,4,true); release(0); tick(0);
  assert(envelopes[0]==0 && kicks[0]==0 && manual[0]==0 && events.empty());
  sequence(1,{});
  // Normal zero-step manual playing and sequencer-mode behavior are preserved.
  reset(); strArp_steps[0]=0; press(0,4);
  assert(envelopes[0]==1 && kicks[0]==1 && manual[0]==1);
  reset(); strArp_modeSel=false; strArp_act=0; genSq_muteCh[0][0]=true;
  press(0,4,true); assert(envelopes[0]==1 && kicks[0]==1);

  // Sync resets both cursors without changing the configured durations.
  reset(); press(0,4); press(1,5,true); tick(0);
  strArp_sync();
  for(const auto &p:strArp_playback)assert(p.step==0 && p.displayStep==-1 && p.nextClock==6);

  // Pattern changes keep channel ownership and durations separate.
  reset(); press(0,4); press(1,5,true); release(1); tick(0);
  strArp_patterns.pttn[1]=strArp_patterns.pttn[0]; strArp_actPttn=1;
  strArp_chn[1]=9; strArp_tmDv[0]=4; strArp_tmDv[1]=8;
  strArp_sync(); tick(1); tick(5);
  assert(count(note(0,4),1,true)==3);
  assert(count(note(1,5),2,false)==1 && count(note(1,5),9,true)==1);
  assert(count(note(1,5),9,false)==0);
  genSq_allNOff();
  assert(count(note(0,4),1,false)==3 && count(note(1,5),9,false)==1);

  // The transport reset tick does not play the first held note early.
  reset(); press(0,4,true); release(0); mClock=-1; strArp_updClck();
  assert(events.empty()); tick(0); assert(count(note(0,4),1,true)==1);

  // The pitchless sensing mode must not emit made-up MIDI pitches.
  reset(); press(0,4,true); release(0); frtb_sensMode=1; tick(0);
  assert(events.empty() && envelopes[0]==0 && kicks[0]==0);

  // Maximum-size mirrored sequences fit both playback and the visible LED grid.
  reset(); press(0,4,true); release(0); press(1,5);
  strArp_steps[0]=strArp_steps[1]=64; strArp_strPrsFnc=strArp_strPrsFnc_mirror;
  tick(0); assert(strArp_playback[0].seqLen==64 && strArp_playback[1].seqLen==64);
  std::cout << "StrHold arpeggiator tests passed\n";
}
'''

source = "\n".join([
    stubs, state, colors, prototypes,
    function(midi, "void sndMidiNote("),
    function(midi, "void sndMidiNotePress("),
    function(midi, "void genSq_allNOff("),
    '#include "' + str(CTL / "09_op02_StrArp.ino") + '"',
    function(hid, "bool stringHasManualEnvelopeDecay("),
    function(hid, "void handleFretEventForAudioMidiKickSeq("),
    tests,
])
with tempfile.TemporaryDirectory(prefix="elektrocaster-arp-") as directory:
    cpp = Path(directory) / "str_arp_test.cpp"
    binary = Path(directory) / "str_arp_test"
    cpp.write_text(source)
    subprocess.run([
        "g++", "-std=gnu++11", "-Wall", "-Wextra", "-Werror",
        "-Wno-unused-parameter", "-fsanitize=address,undefined",
        "-fno-omit-frame-pointer", "-g", str(cpp), "-o", str(binary),
    ], check=True)
    subprocess.run([str(binary)], check=True)
