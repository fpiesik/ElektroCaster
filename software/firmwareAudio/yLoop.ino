#include "ProtocolAudio.h"

namespace {
constexpr unsigned long AUDIO_SERIAL_PACKET_TIMEOUT_MS = 10;
constexpr byte AUDIO_SERIAL_MAX_PAYLOAD = 3;

// CTL -> Audio payload lengths. These preserve the existing byte protocol:
// command byte AUDIO_CMD_* followed by exactly this many payload bytes.
byte audioCommandPayloadLength(byte command) {
  switch (command) {
    case AUDIO_CMD_TRIG_ENV: return 2;
    case AUDIO_CMD_STR_FRET: return 3;
    case AUDIO_CMD_OP_MODE: return 1;
    case AUDIO_CMD_DISP_MODE: return 1;
    case AUDIO_CMD_KICK_MODE: return 1;
    case AUDIO_CMD_BOW_MODE: return 1;
    case AUDIO_CMD_BOW_ON: return 1;
    case AUDIO_CMD_ENV_1: return 2;
    case AUDIO_CMD_ENV_2: return 2;
    case AUDIO_CMD_FILTER: return 2;
    case AUDIO_CMD_MANUAL_ENV: return 2;
    case AUDIO_CMD_VOLUME: return 1;
    case AUDIO_CMD_STR_GAIN: return 2;
    case AUDIO_CMD_FX: return 2;
    case AUDIO_CMD_LFO_1: return 2;
    case AUDIO_CMD_BPM: return 1;
    case AUDIO_CMD_MIDI_CC: return 2;
    default: return 0;
  }
}

void resetAudioSerialPacket(byte &command, byte &payloadIndex, byte &payloadLength) {
  command = 0;
  payloadIndex = 0;
  payloadLength = 0;
}

void handleAudioSerialPacket(byte command, const byte payload[]) {
  int incoming = audioIncoming(command);

  if (incoming == audioIncoming(AUDIO_CMD_TRIG_ENV)) {
    trigEnv(payload[0], payload[1] / 199.0);
  }

  if (incoming == audioIncoming(AUDIO_CMD_STR_FRET)) {
    strFret(payload[0], payload[1], payload[2]);
    strState[payload[0]] = payload[2];
  }

  if (incoming == audioIncoming(AUDIO_CMD_ENV_1)) {
    float sclVal = scale(payload[1] / 199.0, 1, sclEnvA[payload[0]]);
    chEnvA(payload[0], sclVal);
    envPA[payload[0]] = sclVal;
  }

  if (incoming == audioIncoming(AUDIO_CMD_ENV_2)) {
    float sclVal = scale(payload[1] / 199.0, 1, sclEnvF[payload[0]]);
    chEnvF(payload[0], sclVal);
    envPF[payload[0]] = sclVal;
  }

  if (incoming == audioIncoming(AUDIO_CMD_MANUAL_ENV)) {
    chManualEnvDecay(payload[0], payload[1]);
  }

  if (incoming == audioIncoming(AUDIO_CMD_FILTER)) {
    valFilter[payload[0]] = scale(payload[1] / 199.0, 1, sclFilter[payload[0]]);
    chFilter(payload[0], valFilter[payload[0]]);
  }

  if (incoming == audioIncoming(AUDIO_CMD_STR_GAIN)) {
    float val = payload[1] / 100.0;
    chngStrOutGain(payload[0], val);
    DBG_AUDIO_PRINT(payload[0]);
    DBG_AUDIO_PRINT(": ");
    DBG_AUDIO_PRINTLN(val);
  }

  if (incoming == audioIncoming(AUDIO_CMD_FX)) {
    valFX[payload[0]] = scale(payload[1] / 199.0, 1, sclFX[payload[0]]);
    chFX(payload[0], valFX[payload[0]]);
  }

  if (incoming == audioIncoming(AUDIO_CMD_LFO_1)) {
    valLfo1[payload[0]] = scale(payload[1] / 199.0, 1, sclLfo1[payload[0]]);
    for (int i = 0; i < nStrings; i++) {
      chLfo1(payload[0], valLfo1[payload[0]], i);
    }
  }

  if (incoming == audioIncoming(AUDIO_CMD_MIDI_CC)) {
    if (payload[0] == 2) sndMidiCC(3, payload[1] / 2);
  }

  if (payload[0] <= 199) {
    if (incoming == audioIncoming(AUDIO_CMD_OP_MODE)) chOpMode(payload[0]);
    if (incoming == audioIncoming(AUDIO_CMD_DISP_MODE)) chDispMode(payload[0]);
    if (incoming == audioIncoming(AUDIO_CMD_KICK_MODE)) chKickMode(payload[0]);
    if (incoming == audioIncoming(AUDIO_CMD_BOW_MODE)) chBowMode(payload[0]);
    if (incoming == audioIncoming(AUDIO_CMD_BOW_ON)) bowOn = payload[0];

    if (incoming == audioIncoming(AUDIO_CMD_VOLUME)) {
      float val = payload[0] / 199.0;
      val = val * val * 2;
      ampOut.gain(val + 0.0001);
    }

    if (incoming == audioIncoming(AUDIO_CMD_BPM)) {
      bpm = payload[0];
    }
  }
}
}

void serialEvent1(){
  static byte command = 0;
  static byte payload[AUDIO_SERIAL_MAX_PAYLOAD];
  static byte payloadIndex = 0;
  static byte payloadLength = 0;
  static unsigned long packetStartedAt = 0;

  if (payloadLength > 0 && millis() - packetStartedAt > AUDIO_SERIAL_PACKET_TIMEOUT_MS) {
    resetAudioSerialPacket(command, payloadIndex, payloadLength);
  }

  while (Serial1.available() > 0) {
    byte sbyte = Serial1.read();

    if (sbyte >= AUDIO_CMD_BASE && sbyte <= AUDIO_CMD_MAX) {
      command = sbyte;
      payloadIndex = 0;
      payloadLength = audioCommandPayloadLength(command);
      packetStartedAt = millis();
      if (payloadLength == 0 || payloadLength > AUDIO_SERIAL_MAX_PAYLOAD) {
        resetAudioSerialPacket(command, payloadIndex, payloadLength);
      }
      continue;
    }

    if (payloadLength == 0) {
      continue;
    }

    payload[payloadIndex++] = sbyte;
    if (payloadIndex >= payloadLength) {
      handleAudioSerialPacket(command, payload);
      resetAudioSerialPacket(command, payloadIndex, payloadLength);
    }
  }
}

void loop(){
  //usbMIDI.read();

//---Do control functions-----
if (millis()-ctlTimer > ctlInt){ 
  for(int i=0;i<nStrings;i++){
    if(strState[i]!=lastStrState[i]){
      //strFret(i,strState[i]);
      lastStrState[i]=strState[i];
    }
  }
  ctlTimer=millis();
}

if (millis()-nFrqTimer > nFrqInt){ 
static int i=0;
static int cntSmp;
static int nSmp=5;
  for(int s=0;s<nStrings;s++){
    nFreq[s].resume();
    if (nFreq[s].available()) {
        float note = nFreq[s].read();
        float prob = nFreq[s].probability();
        
        float mNote = base_pitch + (12.0 * log(note / base_frequency) / log(2));
        if(prob>0.96){
          strP[s]=mNote;
          //chLfo1(0, mNote,s);
          //chCoilDel(s,note);
//          Serial.printf("freq1: %3.2f | Probability: %.2f\n", note, prob);    
//          Serial.print(i+1);
//          Serial.print(" ");
//          Serial.printf("mNote: %3.2f | Probability: %.2f\n", mNote, prob);
        }               
    
    }
    //nFreq[s].stop(); 
  }
//    cntSmp++;
//    if (cntSmp>=nSmp){
//      nFreq[i].stop();
//      nFreq[(i+1)%nStrings].resume();
//      cntSmp=0;
//      i++;
//      i=i%nStrings;
//    }

  sndStrP();
  
  nFrqTimer=millis();
}

if (millis()-peakTimer > peakInt){ 
  for(int s=0; s<nStrings; s++){
    if (strPeak[s].available()) {
      float vel = strPeak[s].read() * 5.0;
      //Serial.printf("freq1: %3.2f | Probability: %.2f\n", note, prob);
      if(vel>1.0)vel=1.0;  
      strA[s]=vel;  
//      Serial.print(s+1);
//      Serial.print(" ");
//      Serial.print("Vel: "); 
//      Serial.println(vel);          
    } 
  }
  sndStrA();
  peakTimer=millis();
}

if(bowOn!=lastBowOn){
  // for (int i=0; i<nStrings; i++) {
  //   if(bowOn==0)coilAmp[i].gain(0.000);
  //   if(bowOn==1 && strState[i]>0)coilAmp[i].gain(100);
  // }
  for (int str=0; str<nStrings; str++) {
    if(bowOn==1 && strState[str]>0)coilOsc[str].amplitude(1);
    if(bowOn==0)coilOsc[str].amplitude(0);
  }

  DBG_AUDIO_PRINT("bowOn: ");
  DBG_AUDIO_PRINTLN(bowOn);
  lastBowOn=bowOn;
}
   
if(DEBUG_AUDIO) {
  if(millis() - last_time >= 3000) {
    DBG_AUDIO_PRINT("Proc = ");
    DBG_AUDIO_PRINT(AudioProcessorUsage());
    DBG_AUDIO_PRINT(" (");
    DBG_AUDIO_PRINT(AudioProcessorUsageMax());
    DBG_AUDIO_PRINT("),  Mem = ");
    DBG_AUDIO_PRINT(AudioMemoryUsage());
    DBG_AUDIO_PRINT(" (");
    DBG_AUDIO_PRINT(AudioMemoryUsageMax());
    DBG_AUDIO_PRINTLN(")");
    last_time = millis();
  }
}
}
