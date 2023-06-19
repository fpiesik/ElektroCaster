 #include <Audio.h>
//#include "./libs/Audio/Audio.h"
#include <Wire.h>
//#include <SPI.h>
//#include <SD.h>
//#include <SerialFlash.h>
#include <Bounce.h>
//#include <OSCBundle.h>
//#include <OSCBoards.h>
//#include <OSCMessage.h>
//#include "effect_gate.h"
#include "notefreq.h"
//#include "libs/AudioEffectDynamics/effect_dynamics.h"
//#include "effect_tapedelay10tap.h"

const byte nStrings=6;
const int nCtlFrets=17; //how many led frets
byte midiCh=15;

byte bpm=90;
byte opmode=0;
byte kickMode=0;
byte bowMode=0;
byte bowOn=1;
byte lastBowOn=0;
byte lastBowMode=0;
byte dispMode=0;
byte lastDispMode=0;
int waitS=100;
byte strAIn[nStrings]={10,8,6,4,2,0}; //input audio channels of the strings
byte strAOut[nStrings]={10,8,6,4,2,0}; //output audio channels of the string coils
float strInGain[nStrings]={0.65,0.65,0.65,0.65,0.65,0.65};
float tuning[nStrings]={59,54,50,45,40,35}; //{35,40,45,50,54,59};
//int inputHicut[nStrings]={5000,4500,4000,3500,3000,2500}; //{35,40,45,50,54,59};
int inputHicut[nStrings]={10000,10000,10000,10000,10000,10000}; //{35,40,45,50,54,59};
unsigned long ctlTimer;
unsigned int ctlInt=10;

unsigned long nFrqTimer;
unsigned int nFrqInt=20;

unsigned long peakTimer;
unsigned int peakInt=20;

//ControlChangeRoutings
byte ccFilter[4]={53,54,55,56};
float valFilter[4]={0,1,0,4};
float sclFilter[4]={5000,7,3,7};

float valDelay[3]={500,0.5,0.5};
float sclDelay[3]={1,1,1};

float valFX[4]={1,1,1,1};
float sclFX[4]={1,1,1,20};

float valLfo1[5]={0,1,0.5,0.5,0.5};
float sclLfo1[5]={1,1,1,1,1};

byte ccEnvA[7]={102,103,104,105,106,107,108};
byte ccEnvF[7]={109,110,111,112,113,114,115};

float sclEnvA[7]={50,1000,300,5000,1,300,1};
float sclEnvF[7]={50,1000,300,5000,1,300,1};

byte strState[nStrings]={0,0,0,0,0,0};
byte lastStrState[nStrings]={0,0,0,0,0,0};

byte noteState[128];
byte lastNoteState[128];

byte ccState[128];
byte lastCCState[128];

float strP[nStrings];
float lastStrP[nStrings];

float strA[nStrings];
float lastStrA[nStrings];

int tP=440.0; //tuning pitch

//float midiFreq[127];
float midiF;

//convert midi note to frequency
float midiFreq(float note){ 
  return(tP * pow(2,(note-69)/12)/2);
}

float envPA[7]={0,50,0,200,0,80,1};
float envPF[7]={0,50,0,200,0,80,1};

AudioInputTDM   audioInput;
AudioOutputTDM  audioOutput;

AudioMixer4     mixer1;  
AudioMixer4     mixer2;  

AudioMixer4     aBiasM[nStrings];

AudioMixer4     fBiasM[nStrings]; 

AudioMixer4     coilMix[nStrings]; 

AudioMixer4     mainMix;

AudioFilterStateVariable  filter[nStrings]; 

//Amplifiers for the multiBow
AudioAmplifier           coilAmp[nStrings];
//to get more intense sustein effect (eBow)
AudioEffectWaveshaper    coilShaper[nStrings];

AudioAmplifier           ampOut;

AudioAmplifier           inGain[nStrings];

AudioEffectEnvelope      aEnv[nStrings];

AudioEffectEnvelope      fEnv[nStrings];

AudioSynthWaveformModulated lfo[nStrings];

AudioEffectMultiply       ringAmp[nStrings];

AudioSynthWaveformDc     dcFEnv;

AudioAnalyzeMonoFrequency nFreq[nStrings];

AudioAnalyzeRMS  strPeak[nStrings];


AudioConnection*  cbl_input[nStrings]; //connect the inputs with the input gain stage (audioInput, 10, inGain1, 0);
AudioConnection*  cbl_peak[nStrings]; //connect input gain stage to the peak detector  (inGain1, 0, peak1, 0);
AudioConnection*  cbl_pitch[nStrings]; // connect the input stage to the pitch detector (inGain1, 0, nFreq1, 0);
AudioConnection*  cbl_ringAmpSig[nStrings]; //connect the input stage to the ringmod amp (inGain1, 0, mulA1, 0)
AudioConnection*  cbl_ringAmpLFO[nStrings]; // connect the flo to the ringmod amp (lfo1, 0, mulA1, 1);
AudioConnection*  cbl_aBiasDryIn[nStrings]; // connect the ring Amp to the amp bias mixer (mulA1, 0, aBiasM1, 0);
AudioConnection*  cbl_aBiasEnvIn[nStrings]; // connect the amp envelope to the amp bias mixer (aEnv1, 0, aBiasM1, 1);
AudioConnection*  cbl_aEnvRingIn[nStrings]; // connect the ring amp to the amp envelope (mulA1, 0, aEnv1, 0);
AudioConnection*  cbl_filterInSig[nStrings]; //connect bias mixer to the filter audio input (aBiasM1, 0, filter1, 0);
AudioConnection*  cbl_filterInCutoff[nStrings]; //connect the filter bias mixer to filter cutoff input(fBiasM[0], 0, filter1, 1);
AudioConnection*  cbl_fBiasInDC[nStrings]; //connect dc to the filter bias mixer (dcFEnv,0, fBiasM[0], 0);
AudioConnection*  cbl_fEnvInDC[nStrings]; //connect dc to the filter envelope to (dcFEnv, 0, fEnv1, 0);
AudioConnection*  cbl_fBiasInFEnv[nStrings]; //connect the filter envelope to the filter bias mixer (fEnv1, 0, fBiasM[0], 1);
AudioConnection*  cbl_coilShaperIn[nStrings]; //connect the input stage to the coil waveshaper (inGain1, 0, waveshaper1, 0);
AudioConnection*  cbl_coilAmpIn[nStrings]; //connect the coil waveshaper to the coil amp (waveshaper1, 0, cAmp1, 0);
AudioConnection*  cbl_coilAmpOut[nStrings]; //connect the coil amps to the outputs (coilAmp[0],0, audioOutput,10);

//sum all strings to mono output
AudioConnection c7d(filter[0], 0, mixer1, 0);
AudioConnection c8d(filter[1], 0, mixer1, 1);
AudioConnection c9d(filter[2], 0, mixer1, 2);
AudioConnection c10d(filter[3], 0, mixer1, 3);
AudioConnection c11d(mixer1, 0, mixer2, 0);
AudioConnection c12d(filter[4], 0, mixer2, 1);
AudioConnection c13d(filter[5], 0, mixer2, 2);
AudioConnection c52(mixer2, 0, ampOut, 0);
AudioConnection c1200(ampOut,0, audioOutput,12);


AudioControlCS42448 audioShield;


unsigned long last_time = millis();


double base_frequency = 440.0;
double base_pitch = 69.0;


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

void trigEnv(byte str, float val){
  if(val>0){
    //chngStrOutGain(str,val);
    aEnv[str].noteOn();
    fEnv[str].noteOn();
    //usbMIDI.sendNoteOn(strState[str]+tuning[str], 127, 1);
  }
  if(val==0){
    aEnv[str].noteOff();
    fEnv[str].noteOff();
    //chngStrOutGain(str,1);
    //usbMIDI.sendNoteOff(strState[str]+tuning[str], 0, 1);
  }
  
  }

void strFret(byte str, byte fret){
  if(fret==0)coilAmp[str].gain(0);
  if(bowOn==1&&fret>0){
    coilAmp[str].gain(50);
    Serial.print("Str: ");
    Serial.print(str);
    Serial.print(" Fret: ");
    Serial.println(fret);
    //strState[str]=data;   
  }
  //sndMidiNote(str,fret);
  //chLfo1(2,0,str);
}
void chngStrOutGain(int s, float g){
  if(s==0)mixer1.gain(0,g);
  if(s==1)mixer1.gain(1,g);
  if(s==2)mixer1.gain(2,g);
  if(s==3)mixer1.gain(3,g);
  if(s==4)mixer2.gain(1,g);
  if(s==5)mixer2.gain(2,g);
}
