#!/usr/bin/env python3
"""Exercise the actual CTL switch handlers and Audio coil controls on the host."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
CTL = ROOT / "software/firmwareCtl"
AUDIO = ROOT / "software/firmwareAudio"


def function(source, signature):
    start = source.index(signature)
    opening = source.index("{", start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


stubs = r'''
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>
#include <Arduino.h>
constexpr int nStrings=6;
#define DBG_AUDIO_PRINT(x)
#define DBG_AUDIO_PRINTLN(x)
#define STUB(name) template<class... T> void name(T...) {}
namespace Ctl {
struct SerialStub {
  std::vector<byte> bytes;
  void write(byte value) { bytes.push_back(value); }
} Serial1;
bool hidDVal[19], lastHidDVal[19], kickOn, shift, extClk, clckOn;
bool strSeq_act, strArp_act, strArp_modeSel, fbrdMode, fbrdSeqVHld, tgl_ply;
byte frtb_sensMode;
constexpr int genSq_nInst=3, genSq_opMode=2;
int dispEncMode, opMode=2, genSq_actInst, genSq_actSng;
int genSq_edtPttn[3], genSq_lastActPttn[3], genSq_actPttn[3];
int genSq_strEncBtnSw, genSq_strEncFnc, strArp_strEncFnc;
bool schdSync[3];
STUB(genSq_cpPttn)
STUB(sendManualEnvelopeStateForPressedStrings)
STUB(clck_strt)
STUB(clck_stp)
STUB(chOpMode)
STUB(defaultSong)
STUB(loadSong)
STUB(saveSong)
STUB(genSq_chStrBtn)
STUB(strArp_chStrBtn)
}
namespace Audio {
byte bowOn, strState[nStrings], bpm;
float coilgain[nStrings], sclEnvA[7], sclEnvF[7], sclFilter[4], sclFX[4], sclLfo1[5];
float envPA[7], envPF[7], valFilter[4], valFX[4], valLfo1[5];
struct OscillatorStub {
  float level=0;
  void amplitude(float value) { level=value; }
} coilOsc[nStrings];
struct GainStub { void gain(float) {} } ampOut;
float scale(float value, int, float) { return value; }
STUB(trigEnv)
STUB(chEnvA)
STUB(chEnvF)
STUB(chManualEnvDecay)
STUB(chFilter)
STUB(chngStrOutGain)
STUB(chFX)
STUB(chLfo1)
STUB(sndMidiCC)
STUB(chOpMode)
STUB(chDispMode)
STUB(chKickMode)
STUB(chBowMode)
}
'''

tests = r'''
// Bits: Auto=1, MulEBow=2, Kickup=4. Only the all-on state suppresses a Bow request.
const bool expectedBow[8]={false,false,true,true,false,false,true,false};
int switchMask() {
  return Ctl::hidDVal[2] + 2*Ctl::hidDVal[3] + 4*Ctl::hidDVal[4];
}
void checkCoils() {
  bool enabled=expectedBow[switchMask()];
  assert(Audio::bowOn==enabled);
  for(int s=0;s<nStrings;s++) {
    float expected=enabled && Audio::strState[s]>0 ? Audio::coilgain[s] : 0;
    assert(Audio::coilOsc[s].level==expected);
  }
}
void transfer() {
  auto &packet=Ctl::Serial1.bytes;
  if(packet.empty())return;
  assert(packet.size()==2 && packet[0]==206);
  assert(packet[1]==expectedBow[switchMask()]);
  Audio::handleAudioSerialPacket(packet[0],packet.data()+1);
  packet.clear();
}
void setSwitch(int index, bool value) {
  bool previousClock=Ctl::extClk;
  byte previousSensing=Ctl::frtb_sensMode;
  bool changed=Ctl::hidDVal[index]!=value;
  Ctl::rcvHidD(index,value);
  transfer();
  checkCoils();
  assert(Ctl::kickOn==Ctl::hidDVal[4]);
  assert(Ctl::strSeq_act==(Ctl::hidDVal[2] && !Ctl::strArp_modeSel));
  assert(Ctl::strArp_act==(Ctl::hidDVal[2] && Ctl::strArp_modeSel));
  assert(Ctl::extClk==(changed && index==3 && Ctl::shift ? value : previousClock));
  assert(Ctl::frtb_sensMode==(changed && index==4 && Ctl::shift ? value : previousSensing));
}
void reset(bool arpeggiator, bool shift, bool clock) {
  Ctl::Serial1.bytes.clear();
  std::fill_n(Ctl::hidDVal,19,false);
  std::fill_n(Ctl::lastHidDVal,19,false);
  Ctl::kickOn=Ctl::strSeq_act=Ctl::strArp_act=Ctl::extClk=false;
  Ctl::frtb_sensMode=0;
  Ctl::strArp_modeSel=arpeggiator;
  Ctl::shift=shift;
  Ctl::clckOn=clock;
  Audio::bowOn=0;
  for(int s=0;s<nStrings;s++) {
    Audio::strState[s]=s%2;
    Audio::coilgain[s]=(s+1)/20.0f;
    Audio::coilOsc[s].level=0;
  }
}
void fret(byte string, byte state) {
  Ctl::sndStrPrs(string,64,state);
  auto &packet=Ctl::Serial1.bytes;
  assert(packet.size()==4 && packet[0]==201);
  Audio::handleAudioSerialPacket(packet[0],packet.data()+1);
  packet.clear();
  checkCoils();
}
int main() {
  int transitions=0;
  for(bool arp:{false,true})for(bool shift:{false,true})for(bool clock:{false,true}) {
    for(int from=0;from<8;from++)for(int to=0;to<8;to++) {
      std::array<int,3> order={{0,1,2}};
      do {
        reset(arp,shift,clock);
        for(int i=0;i<3;i++)setSwitch(i+2,(from & (1<<i))!=0);
        for(int i:order)setSwitch(i+2,(to & (1<<i))!=0);
        assert(switchMask()==to);
        transitions++;
      } while(std::next_permutation(order.begin(),order.end()));
    }

    // Existing coil signals stop immediately; new fret events stay silent while blocked.
    reset(arp,shift,clock);
    setSwitch(3,true);
    setSwitch(2,true);
    setSwitch(4,true);
    for(byte s=0;s<nStrings;s++)fret(s,1);
    fret(1,0); fret(4,0);
    setSwitch(2,false); // Resume only the strings still fretted, without new contacts.
    fret(0,0);

    // Releasing Kickup also restores Bow; an off Bow switch must stay off afterward.
    setSwitch(2,true);
    setSwitch(4,false);
    setSwitch(4,true);
    setSwitch(3,false);
    setSwitch(2,false);
    setSwitch(4,false);
  }
  std::cout << "MulEBow control tests passed (" << transitions
            << " switch transition sequences, plus active-coil and fret-event cases)\n";
}
'''

hid = (CTL / "04_hid.ino").read_text()
senders = (CTL / "2audio.ino").read_text()
audio = (AUDIO / "firmwareAudio.ino").read_text()
audio_loop = (AUDIO / "yLoop.ino").read_text()
source = "\n".join([
    '#include "' + str(AUDIO / "ProtocolAudio.h") + '"',
    stubs,
    "namespace Ctl {",
    function(senders, "void sndBowOn("),
    function(senders, "void sndStrPrs("),
    function(hid, "void procHidDChng("),
    function(hid, "void rcvHidD("),
    "}",
    "namespace Audio {",
    function(audio, "void chBowOn("),
    function(audio, "void strFret("),
    function(audio_loop, "void handleAudioSerialPacket("),
    "}",
    tests,
])
with tempfile.TemporaryDirectory(prefix="elektrocaster-bow-") as directory:
    directory = Path(directory)
    (directory / "Arduino.h").write_text("#pragma once\nusing byte = unsigned char;\n")
    cpp = directory / "bow_control_test.cpp"
    binary = directory / "bow_control_test"
    cpp.write_text(source)
    subprocess.run([
        "g++", "-std=gnu++11", "-Wall", "-Wextra", "-Werror",
        "-Wno-unused-parameter", "-fsanitize=address,undefined",
        "-fno-omit-frame-pointer", "-g", "-I", str(directory),
        str(cpp), "-o", str(binary),
    ], check=True)
    subprocess.run([str(binary)], check=True)
