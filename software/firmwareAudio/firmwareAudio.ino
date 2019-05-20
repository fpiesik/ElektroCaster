#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCMessage.h>
#include <AsciiMassageParser.h>
 AsciiMassageParser inbound;
#include <AsciiMassagePacker.h>
AsciiMassagePacker outbound;

const byte nStrings=6;
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

#define DELAY_MAX_LEN 10000
short tape_delay_bank[DELAY_MAX_LEN] = {};

AudioInputTDM  audioInput;
AudioOutputTDM  audioOutput;
//AudioOutputUSB  usbOut;
AudioMixer4     mixer1;  
AudioMixer4     mixer2; 
AudioMixer4     mixer3; 
AudioMixer4     mixer4; 

AudioFilterStateVariable  filter1; 
AudioFilterStateVariable  filter2; 
AudioFilterStateVariable  filter3; 
AudioFilterStateVariable  filter4; 
AudioFilterStateVariable  filter5; 
AudioFilterStateVariable  filter6; 

AudioAmplifier           amp1;
AudioAmplifier           amp2;
AudioAmplifier           amp3;
AudioAmplifier           amp4;
AudioAmplifier           amp5;
AudioAmplifier           amp6;
AudioAmplifier           ampOut;
AudioAmplifier           gain1;
AudioAmplifier           gain2;
AudioAmplifier           gain3;
AudioAmplifier           gain4;
AudioAmplifier           gain5;
AudioAmplifier           gain6;
AudioEffectEnvelope      env1;
AudioEffectEnvelope      env2;
AudioEffectEnvelope      env3;
AudioEffectEnvelope      env4;
AudioEffectEnvelope      env5;
AudioEffectEnvelope      env6;
AudioEffectEnvelope      env7;
AudioEffectEnvelope      env8;
AudioEffectEnvelope      env9;
AudioEffectEnvelope      env10;
AudioEffectEnvelope      env11;
AudioEffectEnvelope      env12;
AudioSynthWaveform       wave1;
AudioSynthWaveform       wave2;
AudioSynthWaveform       wave3;
AudioSynthWaveform       wave4;
AudioSynthWaveform       wave5;
AudioSynthWaveform       wave6; 
AudioEffectWaveshaper    waveshaper1;
AudioEffectWaveshaper    waveshaper2;
AudioEffectWaveshaper    waveshaper3;
AudioEffectWaveshaper    waveshaper4;
AudioEffectWaveshaper    waveshaper5;
AudioEffectWaveshaper    waveshaper6;
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
AudioEffectTapeDelay     delay0; 
AudioSynthWaveformDc     dcEnv2;

AudioEffectEnvelope      *envs[nStrings] = {
  &env1, &env2, &env3,
  &env4, &env5, &env6 
};
AudioEffectEnvelope      *envs2[nStrings] = {
  &env7, &env8, &env9,
  &env10, &env11, &env12 
};
AudioAmplifier           *amps[nStrings]={
  &amp1, &amp2, &amp3,
  &amp4, &amp5, &amp6
};
AudioAmplifier           *gains[nStrings]={
  &gain1, &gain2, &gain3,
  &gain4, &gain5, &gain6
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



//test filter
AudioConnection c16(audioInput, 2, gain1, 0);
AudioConnection c17(audioInput, 0, gain2, 0);
AudioConnection c18(audioInput, 4, gain3, 0);
AudioConnection c19(audioInput, 10, gain4, 0);
AudioConnection c20(audioInput, 6, gain5, 0);
AudioConnection c21(audioInput, 8, gain6, 0);

//AudioConnection c41(gain1, 0, mulA1, 0);
//AudioConnection c42(gain2, 0, mulA2, 0);
//AudioConnection c43(gain3, 0, mulA3, 0);
//AudioConnection c44(gain4, 0, mulA4, 0);
//AudioConnection c45(gain5, 0, mulA5, 0);
//AudioConnection c46(gain6, 0, mulA6, 0);

AudioConnection c41(gain1, 0, env1, 0);
AudioConnection c42(gain2, 0, env2, 0);
AudioConnection c43(gain3, 0, env3, 0);
AudioConnection c44(gain4, 0, env4, 0);
AudioConnection c45(gain5, 0, env5, 0);
AudioConnection c46(gain6, 0, env6, 0);

//AudioConnection c66(lfo1, 0, mulA1, 1);
//AudioConnection c67(lfo2, 0, mulA2, 1);
//AudioConnection c68(lfo3, 0, mulA3, 1);
//AudioConnection c69(lfo4, 0, mulA4, 1);
//AudioConnection c70(lfo5, 0, mulA5, 1);
//AudioConnection c71(lfo6, 0, mulA6, 1);
//
//AudioConnection c72(mulA1, 0, env1, 0);
//AudioConnection c73(mulA2, 0, env2, 0);
//AudioConnection c74(mulA3, 0, env3, 0);
//AudioConnection c75(mulA4, 0, env4, 0);
//AudioConnection c76(mulA5, 0, env5, 0);
//AudioConnection c77(mulA6, 0, env6, 0);

AudioConnection c1(env1, 0, filter1, 0);
AudioConnection c2(env2, 0, filter2, 0);
AudioConnection c3(env3, 0, filter3, 0);
AudioConnection c4(env4, 0, filter4, 0);
AudioConnection c5(env5, 0, filter5, 0);
AudioConnection c6(env6, 0, filter6, 0);

AudioConnection c7(filter1, 0, mixer1, 0);
AudioConnection c8(filter2, 0, mixer1, 1);
AudioConnection c9(filter3, 0, mixer1, 2);
AudioConnection c10(filter4, 0, mixer1, 3);
AudioConnection c11(mixer1, 0, mixer2, 0);
AudioConnection c12(filter5, 0, mixer2, 1);
AudioConnection c13(filter6, 0, mixer2, 2);


AudioConnection c22(amp1, 0, audioOutput, 0);
AudioConnection c23(amp2, 0, audioOutput, 2);
AudioConnection c24(amp3, 0, audioOutput, 4);
AudioConnection c25(amp4, 0, audioOutput, 6);
AudioConnection c26(amp5, 0, audioOutput, 8);
AudioConnection c27(amp6, 0, audioOutput, 10);


AudioConnection c29(audioInput, 2, waveshaper1, 0);
AudioConnection c30(waveshaper1, 0, amp1, 0);

AudioConnection c31(audioInput, 0, waveshaper2, 0);
AudioConnection c32(waveshaper2, 0, amp2, 0);

AudioConnection c33(audioInput, 4, waveshaper3, 0);
AudioConnection c34(waveshaper3, 0, amp3, 0);

AudioConnection c35(audioInput, 10, waveshaper4, 0);
//AudioConnection c32(audioInput, 10, delay1, 0);
AudioConnection c36(waveshaper4, 0, amp4, 0);


AudioConnection c37(audioInput, 6, waveshaper5, 0);
AudioConnection c38(waveshaper5, 0, amp5, 0);

AudioConnection c39(audioInput, 8, waveshaper6, 0);
AudioConnection c40(waveshaper6, 0, amp6, 0);




AudioConnection c54(dcEnv2,0, env7, 0);
AudioConnection c55(dcEnv2, 0, env8, 0);
AudioConnection c56(dcEnv2, 0, env9, 0);
AudioConnection c57(dcEnv2, 0, env10, 0);
AudioConnection c58(dcEnv2, 0, env11, 0);
AudioConnection c59(dcEnv2, 0, env12, 0);

AudioConnection c60(env7, 0, filter1, 1);
AudioConnection c61(env8, 0, filter2, 1);
AudioConnection c62(env9, 0, filter3, 1);
AudioConnection c63(env10, 0, filter4, 1);
AudioConnection c64(env11, 0, filter5, 1);
AudioConnection c65(env12, 0, filter6, 1);

AudioConnection c47(mixer2, 0, mixer4, 0);
AudioConnection c48(mixer2, 0, mixer3, 0);
AudioConnection c49(mixer3, 0, delay0 , 0);
AudioConnection c50(delay0, 0, mixer3 , 1);
AudioConnection c51(delay0, 0, mixer4 , 1);
AudioConnection c52(mixer4, 0, ampOut, 0);
AudioConnection c53(ampOut, 0, audioOutput, 14);
//AudioConnection c52(mixer2, 0, ampOut, 0);

//ControlChangeRoutings
byte ccFilter[2]={1,2};
float valFilter[4]={0,1,0,4};
float sclFilter[4]={5000,7,3,7};

float valDelay[3]={500,0.5,0.5};
float sclDelay[3]={1,1,1};

float valLfo1[2]={5,1};
float sclLfo1[2]={50,1};

byte ccEnv1[6]={3,4,5,6,7,8};
float valEnv1[6]={0,50,0,200,0.3,80};
float sclEnv1[6]={50,300,300,300,1,300};

float valEnv2[6]={0,50,0,200,0.3,80};
float sclEnv2[6]={50,300,300,300,1,300};

float strGain[nStrings]={2,1,1,1,1,1};

byte strState[nStrings]={0,0,0,0,0,0};

byte noteState[128];
byte lastNoteState[128];

int waitS=500; //wait after serial send

AudioControlCS42448 audioShield;


unsigned long last_time = millis();



float envP[6]={0,50,0,200,0.3,80};
float envF[6]={0,50,0,200,0.3,80};

void chOpMode(byte data){
  opmode=data;
  for (int i=0; i<nStrings; i++){
    //if(opmode==0)envs[i]->sustain(1);
    //if(opmode==0)envs2[i]->sustain(1);
    //if(opmode>0)envs2[i]->sustain(envP[4]);
    if(opmode==0){
      envs[i]->sustain(1);
      envs[i]->noteOn();
      envs2[i]->sustain(1);
      envs2[i]->noteOn();
    }
    if(opmode>0){
      envs[i]->sustain(envP[4]);
      envs[i]->noteOff();
      envs2[i]->sustain(envP[4]);
      envs2[i]->noteOff();
    }
  }
  
  Serial.print("opmode-t: ");
  Serial.println(opmode);
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
    envs[str]->noteOn();
    envs2[str]->noteOn();
      usbMIDI.sendNoteOn(60+str, 127, 1);
  }
  if(val==0){
    envs[str]->noteOff();
    envs2[str]->noteOff();
    //usbMIDI.sendNoteOff(60+str, 0, 1);
  }
  
  }

void strFret(byte str, float data){
  if(1){
  if(data==0)amps[str]->gain(0);
  if(bowOn==1&&data>0)amps[str]->gain(50);
  strState[str]=data;
  }
}
