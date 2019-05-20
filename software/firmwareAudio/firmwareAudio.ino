#include <Audio.h>
//#include "./libs/Audio/Audio.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCMessage.h>
#include "effect_gate.h"
//#include "libs/AudioEffectDynamics/effect_dynamics.h"

const byte nStrings=6;
const int nCtlFrets=17; //how many led frets
byte midiCh=15;

int cutoff=3500;
float q=1;
float ampGain=20;

byte opmode=0;
byte kickMode=0;
byte bowMode=0;
byte bowOn=0;
byte lastBowOn=0;
byte lastBowMode=0;
byte dispMode=0;
byte lastDispMode=0;
int waitS=20;
float tuning[nStrings]={59,54,50,45,40,35}; //{35,40,45,50,54,59};
unsigned long ctlTimer;
unsigned int ctlInt=10;

//ControlChangeRoutings
byte ccFilter[4]={1,2,3,4};
float valFilter[4]={0,1,0,4};
float sclFilter[4]={5000,7,3,7};

float valDelay[3]={500,0.5,0.5};
float sclDelay[3]={1,1,1};

float valFX[4]={1,1,1,1};
float sclFX[4]={1,1,1,20};

float valLfo1[2]={60,1};
float sclLfo1[2]={127,1};

byte ccEnv1[7]={5,6,7,8,9,10,11};

float sclEnvA[7]={50,1000,300,1000,1,300,1};
float sclEnvF[7]={50,1000,300,1000,1,300,1};

float strGain[nStrings]={1.7,1.2,1,1,1,1};

byte strState[nStrings]={0,0,0,0,0,0};
byte lastStrState[nStrings]={0,0,0,0,0,0};

byte noteState[128];
byte lastNoteState[128];

byte ccState[128];
byte lastCCState[128];

int tP=440.0; //tuning pitch

//float midiFreq[127];
float midiF;
void midiFreq(float note){ 
  midiF = tP * pow(2,(note-69)/12);
}
float envPA[6]={0,50,0,200,0,80};
float envPF[6]={0,50,0,200,0,80};


#define DELAY_MAX_LEN 10000
//short tape_delay_bank[DELAY_MAX_LEN] = {};

AudioInputTDM   audioInput;
AudioOutputTDM  audioOutput;
//AudioInputUSB   usbIn;
//AudioOutputUSB  usbOut;
AudioMixer4     mixer1;  
AudioMixer4     mixer2; 
//AudioMixer4     mixer3; 
//AudioMixer4     mixer4; 


AudioMixer4     aBiasM1;
AudioMixer4     aBiasM2;
AudioMixer4     aBiasM3;
AudioMixer4     aBiasM4;
AudioMixer4     aBiasM5;
AudioMixer4     aBiasM6;

AudioMixer4     fBiasM1;
AudioMixer4     fBiasM2;
AudioMixer4     fBiasM3;
AudioMixer4     fBiasM4;
AudioMixer4     fBiasM5;
AudioMixer4     fBiasM6;

AudioMixer4     outMix1;  
AudioMixer4     outMix2;
AudioMixer4     outMix3;
AudioMixer4     outMix4;
AudioMixer4     outMix5;
AudioMixer4     outMix6;
AudioMixer4     outMix7;
AudioMixer4     outMix8;

AudioMixer4     mainMix;

AudioFilterStateVariable  filter1; 
AudioFilterStateVariable  filter2; 
AudioFilterStateVariable  filter3; 
AudioFilterStateVariable  filter4; 
AudioFilterStateVariable  filter5; 
AudioFilterStateVariable  filter6; 

//Amplifiers for the multiBow
AudioAmplifier           cAmp1;
AudioAmplifier           cAmp2;
AudioAmplifier           cAmp3;
AudioAmplifier           cAmp4;
AudioAmplifier           cAmp5;
AudioAmplifier           cAmp6;

//to get more intense sustein effect
AudioEffectWaveshaper    waveshaper1;
AudioEffectWaveshaper    waveshaper2;
AudioEffectWaveshaper    waveshaper3;
AudioEffectWaveshaper    waveshaper4;
AudioEffectWaveshaper    waveshaper5;
AudioEffectWaveshaper    waveshaper6;

AudioAmplifier           ampOut;

AudioAmplifier           inGain1;
AudioAmplifier           inGain2;
AudioAmplifier           inGain3;
AudioAmplifier           inGain4;
AudioAmplifier           inGain5;
AudioAmplifier           inGain6;

AudioAmplifier           preA1;
AudioAmplifier           preA2;
AudioAmplifier           preA3;
AudioAmplifier           preA4;
AudioAmplifier           preA5;
AudioAmplifier           preA6;

AudioEffectEnvelope      aEnv1;
AudioEffectEnvelope      aEnv2;
AudioEffectEnvelope      aEnv3;
AudioEffectEnvelope      aEnv4;
AudioEffectEnvelope      aEnv5;
AudioEffectEnvelope      aEnv6;

AudioEffectEnvelope      fEnv1;
AudioEffectEnvelope      fEnv2;
AudioEffectEnvelope      fEnv3;
AudioEffectEnvelope      fEnv4;
AudioEffectEnvelope      fEnv5;
AudioEffectEnvelope      fEnv6;

AudioSynthWaveform       not0;



//to smooth clipping
AudioEffectWaveshaper    ws1;
AudioEffectWaveshaper    ws2;
AudioEffectWaveshaper    ws3;
AudioEffectWaveshaper    ws4;
AudioEffectWaveshaper    ws5;
AudioEffectWaveshaper    ws6;

AudioSynthWaveformModulated lfo1;
AudioSynthWaveformModulated lfo2;
AudioSynthWaveformModulated lfo3;
AudioSynthWaveformModulated lfo4;
AudioSynthWaveformModulated lfo5;
AudioSynthWaveformModulated lfo6;

AudioEffectMultiply       mulA1;
AudioEffectMultiply       mulA2;
AudioEffectMultiply       mulA3;
AudioEffectMultiply       mulA4;
AudioEffectMultiply       mulA5;
AudioEffectMultiply       mulA6;

AudioEffectFreeverb       verb; 

AudioSynthWaveformDc     dcFEnv;

AudioEffectEnvelope      *aEnvs[nStrings] = {
  &aEnv1, &aEnv2, &aEnv3,
  &aEnv4, &aEnv5, &aEnv6 
};
AudioEffectEnvelope      *fEnvs[nStrings] = {
  &fEnv1, &fEnv2, &fEnv3,
  &fEnv4, &fEnv5, &fEnv6 
};
AudioAmplifier           *cAmps[nStrings]={
  &cAmp1, &cAmp2, &cAmp3,
  &cAmp4, &cAmp5, &cAmp6
};
AudioAmplifier           *inGains[nStrings]={
  &inGain1, &inGain2, &inGain3,
  &inGain4, &inGain5, &inGain6
};
AudioAmplifier           *preA[nStrings]={
  &preA1, &preA2, &preA3,
  &preA4, &preA5, &preA6
};
AudioFilterStateVariable        *filter[nStrings]={
  &filter1, &filter2, &filter3, 
  &filter4, &filter5, &filter6 
};
AudioSynthWaveformModulated     *lfos1[nStrings]={
  &lfo1, &lfo2, &lfo3,
  &lfo4, &lfo5, &lfo6
};
AudioEffectMultiply     *mulA[nStrings]={
  &mulA1, &mulA2, &mulA3,
  &mulA4, &mulA5, &mulA6
};

AudioEffectWaveshaper   *ws[nStrings]={
  &ws1, &ws2, &ws3,
  &ws4, &ws5, &ws6
};

AudioMixer4   *aBiasM[nStrings]={
  &aBiasM1, &aBiasM2, &aBiasM3,
  &aBiasM4, &aBiasM5, &aBiasM6
};

AudioMixer4   *fBiasM[nStrings]={
  &fBiasM1, &fBiasM2, &fBiasM3,
  &fBiasM4, &fBiasM5, &fBiasM6
};


AudioConnection c16(audioInput, 2, inGain1, 0);
AudioConnection c17(audioInput, 0, inGain2, 0);
AudioConnection c18(audioInput, 4, inGain3, 0);
AudioConnection c19(audioInput, 10, inGain4, 0);
AudioConnection c20(audioInput, 6, inGain5, 0);
AudioConnection c21(audioInput, 8, inGain6, 0);

AudioConnection c1100(inGain1, 0, preA1, 0);
AudioConnection c1101(inGain2, 0, preA2, 0);
AudioConnection c1102(inGain3, 0, preA3, 0);
AudioConnection c1103(inGain4, 0, preA4, 0);
AudioConnection c1104(inGain5, 0, preA5, 0);
AudioConnection c1105(inGain6, 0, preA6, 0);

AudioConnection c141(preA1, 0, ws1, 0);
AudioConnection c142(preA2, 0, ws2, 0);
AudioConnection c143(preA3, 0, ws3, 0);
AudioConnection c144(preA4, 0, ws4, 0);
AudioConnection c145(preA5, 0, ws5, 0);
AudioConnection c146(preA6, 0, ws6, 0);

AudioConnection c041(ws1, 0, mulA1, 0);
AudioConnection c042(ws2, 0, mulA2, 0);
AudioConnection c043(ws3, 0, mulA3, 0);
AudioConnection c044(ws4, 0, mulA4, 0);
AudioConnection c045(ws5, 0, mulA5, 0);
AudioConnection c046(ws6, 0, mulA6, 0);

AudioConnection c66(lfo1, 0, mulA1, 1);
AudioConnection c67(lfo2, 0, mulA2, 1);
AudioConnection c68(lfo3, 0, mulA3, 1);
AudioConnection c69(lfo4, 0, mulA4, 1);
AudioConnection c70(lfo5, 0, mulA5, 1);
AudioConnection c71(lfo6, 0, mulA6, 1);
//
//AudioConnection c72(mulA1, 0, aEnv1, 0);
//AudioConnection c73(mulA2, 0, aEnv2, 0);
//AudioConnection c74(mulA3, 0, aEnv3, 0);
//AudioConnection c75(mulA4, 0, aEnv4, 0);
//AudioConnection c76(mulA5, 0, aEnv5, 0);
//AudioConnection c77(mulA6, 0, aEnv6, 0);

AudioConnection c72(mulA1, 0, aBiasM1, 0);
AudioConnection c73(mulA2, 0, aBiasM2, 0);
AudioConnection c74(mulA3, 0, aBiasM3, 0);
AudioConnection c75(mulA4, 0, aBiasM4, 0);
AudioConnection c76(mulA5, 0, aBiasM5, 0);
AudioConnection c77(mulA6, 0, aBiasM6, 0);

AudioConnection c82(mulA1, 0, aEnv1, 0);
AudioConnection c83(mulA2, 0, aEnv2, 0);
AudioConnection c84(mulA3, 0, aEnv3, 0);
AudioConnection c85(mulA4, 0, aEnv4, 0);
AudioConnection c86(mulA5, 0, aEnv5, 0);
AudioConnection c87(mulA6, 0, aEnv6, 0);

AudioConnection c91(aEnv1, 0, aBiasM1, 1);
AudioConnection c92(aEnv2, 0, aBiasM2, 1);
AudioConnection c93(aEnv3, 0, aBiasM3, 1);
AudioConnection c94(aEnv4, 0, aBiasM4, 1);
AudioConnection c95(aEnv5, 0, aBiasM5, 1);
AudioConnection c96(aEnv6, 0, aBiasM6, 1);

AudioConnection c231(aBiasM1, 0, filter1, 0);
AudioConnection c232(aBiasM2, 0, filter2, 0);
AudioConnection c233(aBiasM3, 0, filter3, 0);
AudioConnection c234(aBiasM4, 0, filter4, 0);
AudioConnection c235(aBiasM5, 0, filter5, 0);
AudioConnection c236(aBiasM6, 0, filter6, 0);

AudioConnection c7(filter1, 0, mixer1, 0);
AudioConnection c8(filter2, 0, mixer1, 1);
AudioConnection c9(filter3, 0, mixer1, 2);
AudioConnection c10(filter4, 0, mixer1, 3);
AudioConnection c11(mixer1, 0, mixer2, 0);
AudioConnection c12(filter5, 0, mixer2, 1);
AudioConnection c13(filter6, 0, mixer2, 2);


AudioConnection c22(cAmp1, 0, outMix1, 0);
AudioConnection c23(cAmp2, 0, outMix2, 0);
AudioConnection c24(cAmp3, 0, outMix3, 0);
AudioConnection c25(cAmp4, 0, outMix4, 0);
AudioConnection c26(cAmp5, 0, outMix5, 0);
AudioConnection c27(cAmp6, 0, outMix6, 0);


AudioConnection c29(audioInput, 2, waveshaper1, 0);
AudioConnection c30(waveshaper1, 0, cAmp1, 0);

AudioConnection c31(audioInput, 0, waveshaper2, 0);
AudioConnection c32(waveshaper2, 0, cAmp2, 0);

AudioConnection c33(audioInput, 4, waveshaper3, 0);
AudioConnection c34(waveshaper3, 0, cAmp3, 0);

AudioConnection c35(audioInput, 10, waveshaper4, 0);
//AudioConnection c32(audioInput, 10, delay1, 0);
AudioConnection c36(waveshaper4, 0, cAmp4, 0);


AudioConnection c37(audioInput, 6, waveshaper5, 0);
AudioConnection c38(waveshaper5, 0, cAmp5, 0);

AudioConnection c39(audioInput, 8, waveshaper6, 0);
AudioConnection c40(waveshaper6, 0, cAmp6, 0);


AudioConnection c254(dcFEnv,0, fBiasM1, 0);
AudioConnection c255(dcFEnv, 0, fBiasM2, 0);
AudioConnection c256(dcFEnv, 0, fBiasM3, 0);
AudioConnection c257(dcFEnv, 0, fBiasM4, 0);
AudioConnection c258(dcFEnv, 0, fBiasM5, 0);
AudioConnection c259(dcFEnv, 0, fBiasM6, 0);

AudioConnection c54(dcFEnv, 0, fEnv1, 0);
AudioConnection c55(dcFEnv, 0, fEnv2, 0);
AudioConnection c56(dcFEnv, 0, fEnv3, 0);
AudioConnection c57(dcFEnv, 0, fEnv4, 0);
AudioConnection c58(dcFEnv, 0, fEnv5, 0);
AudioConnection c59(dcFEnv, 0, fEnv6, 0);

AudioConnection c260(fEnv1, 0, fBiasM1, 1);
AudioConnection c261(fEnv2, 0, fBiasM2, 1);
AudioConnection c262(fEnv3, 0, fBiasM3, 1);
AudioConnection c263(fEnv4, 0, fBiasM4, 1);
AudioConnection c264(fEnv5, 0, fBiasM5, 1);
AudioConnection c265(fEnv6, 0, fBiasM6, 1);

AudioConnection c270(fBiasM1, 0, filter1, 1);
AudioConnection c271(fBiasM2, 0, filter2, 1);
AudioConnection c272(fBiasM3, 0, filter3, 1);
AudioConnection c273(fBiasM4, 0, filter4, 1);
AudioConnection c274(fBiasM5, 0, filter5, 1);
AudioConnection c275(fBiasM6, 0, filter6, 1);

//AudioConnection c47(mixer2, 0, mixer4, 0);
//AudioConnection c48(mixer2, 0, mixer3, 0);
//AudioConnection c49(mixer3, 0, delay0 , 0);
//AudioConnection c50(delay0, 0, mixer3 , 1);
//AudioConnection c51(delay0, 0, mixer4 , 1);
//AudioConnection c52(mixer4, 0, ampOut, 0);

AudioConnection c052(mixer2, 0, verb, 0);
AudioConnection c53(mixer2, 0, outMix8, 0);
AudioConnection c053(verb, 0, outMix8, 1);
AudioConnection c52(outMix8, 0, ampOut, 0);

AudioConnection c1000(not0,0, outMix1,3);
AudioConnection c1001(not0,0, outMix2,3);
AudioConnection c1002(not0,0, outMix3,3);
AudioConnection c1003(not0,0, outMix4,3);
AudioConnection c1004(not0,0, outMix5,3);
AudioConnection c1005(not0,0, outMix6,3);
AudioConnection c1006(not0,0, outMix7,3);
AudioConnection c1007(not0,0, outMix8,3);

AudioConnection c1008(outMix1,0, audioOutput,0);
AudioConnection c1009(outMix2,0, audioOutput,2);
AudioConnection c1010(outMix3,0, audioOutput,4);
AudioConnection c1011(outMix4,0, audioOutput,6);
AudioConnection c1012(outMix5,0, audioOutput,8);
AudioConnection c1013(outMix6,0, audioOutput,10);
AudioConnection c1014(outMix7,0, audioOutput,12);


AudioConnection c1200(ampOut,0, mainMix,0);
//AudioConnection c1201(usbIn,0, mainMix,2);

AudioConnection c1015(mainMix,0, audioOutput,14);
//AudioConnection c1202(mainMix,0, usbOut,0);

AudioControlCS42448 audioShield;


unsigned long last_time = millis();





void chOpMode(byte data){
  opmode=data;
  for (int i=0; i<nStrings; i++){
    //if(opmode==0)envs[i]->sustain(1);
    //if(opmode==0)envs2[i]->sustain(1);
    //if(opmode>0)envs2[i]->sustain(envPA[4]);
    if(opmode==0){
//      aEnvs[i]->sustain(1);
//      aEnvs[i]->noteOn();
//      fEnvs[i]->sustain(1);
//      fEnvs[i]->noteOn();
    }
    if(opmode>0){
      //aEnvs[i]->sustain(envPA[4]);
      //aEnvs[i]->noteOff();
      //fEnvs[i]->sustain(envPF[4]);
      //fEnvs[i]->noteOff();
    }
  }
  }

void chKickMode(byte data){
  kickMode=data;
  Serial.print("kickmode-t: ");
  Serial.println(kickMode);
  }

void chBowMode(byte data){
  bowMode=data;
  Serial.print("bowmode: ");
  Serial.println(bowMode);
  }

void chDispMode(byte data){
  dispMode=data;
  Serial.print("dispmode: ");
  Serial.println(dispMode);
  }

void seqHit(byte str, float val){
  if(val>0){
    aEnvs[str]->noteOn();
    fEnvs[str]->noteOn();
    //usbMIDI.sendNoteOn(strState[str]+tuning[str], 127, 1);
  }
  if(val==0){
    aEnvs[str]->noteOff();
    fEnvs[str]->noteOff();
    //usbMIDI.sendNoteOff(strState[str]+tuning[str], 0, 1);
  }
  
  }

void strFret(byte str, byte fret){
  if(fret==0)cAmps[str]->gain(0);
  if(bowOn==1&&fret>0)cAmps[str]->gain(20);
  //strState[str]=data;
  //sndMidiNote(str,fret);
  chLfo1(2,0,str);
}

void sndCC(){
  for(byte i = 0;i<=127;i++){
    if(ccState[i]!=lastCCState[i]){
      Serial5.write(205);
      delayMicroseconds(waitS);
      Serial5.write(i);
      delayMicroseconds(waitS);
      Serial5.write(ccState[i]);
      delayMicroseconds(waitS);
      lastCCState[i]=ccState[i];  
    }
  }
}
