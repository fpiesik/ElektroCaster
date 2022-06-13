#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
#include <Wire.h>
#include <EEPROM.h>
#include <AsciiMassagePacker.h>
#include <SD.h>
#include <SPI.h>
#include <MIDI.h>;


//midi
MIDI_CREATE_INSTANCE(HardwareSerial, Serial6, MIDI); 

//sdCard
File myFile;
const int chipSelect = BUILTIN_SDCARD;

//string defintions variables
  const int nStrings=6; //how many Strings
  const int nFrets=22;
  byte tuning[nStrings]={64,59,55,50,45,40};
  const byte strSnsPins[nStrings]={2,3,4,5,6,7};
  unsigned long lastFretRead[nStrings];
  unsigned int fretMaskT=35; //time until a next strPres on he same string is detected
  unsigned int strBncs=150; //number of same samples to trigger strPres
  float strP[nStrings];
  float lastStrP[nStrings];
  float strA[nStrings];
  float lastStrA[nStrings];

//led defintions and variables
  #define LED_PIN     33   //led pin
  #define NUMPIXELS    150  //total number of leds
  const int nLedFrets=25; //how many led frets
  //long sndLedTimer;
  //int sndLedInt=1000;
  float tnClrs[12][3];
  float fled_bright = 1; //brightness
  float fled_redC = 150;  //compensate the "filament filter"
  float fled_greenC= 255; //compensate the "filament filter"
  float fled_blueC = 255; //compensate the "filament filter"
  float fled_trgtC[nStrings][nLedFrets][3]; //color to reach
  float fled_actC[nStrings][nLedFrets][3];  //current color
  unsigned long fled_frameTimer; //timer for the led update
  unsigned int fled_frameInt=50;
  float hsbRGB[3]={0,0,0};
  CRGB frtPix[NUMPIXELS];
  CRGB lastFrtPix[NUMPIXELS];
  float trgtC[nStrings][nLedFrets][3];
  float actC[nStrings][nLedFrets][3];
  const byte led_pixPos[nStrings][nLedFrets]={
  {144,143,132,131,120,119,108,107,96,95,84,83,72,71,60,59,48,47,36,35,24,23,12,11,0}, //for 25 frets
  {145,142,133,130,121,118,109,106,97,94,85,82,73,70,61,58,49,46,37,34,25,22,13,10,1}, //for 25 frets
  {146,141,134,129,122,117,110,105,98,93,86,81,74,69,62,57,50,45,38,33,26,21,14,9,2}, //for 25 frets
  {147,140,135,128,123,116,111,104,99,92,87,80,75,68,63,56,51,44,39,32,27,20,15,8,3}, //for 25 frets
  {148,139,136,127,124,115,112,103,100,91,88,79,76,67,64,55,52,43,40,31,28,19,16,7,4}, //for 25 frets
  {149,138,137,126,125,114,113,102,101,90,89,78,77,66,65,54,53,42,41,30,29,18,17,6,5}}; //for 25 frets

//hid
  float hidAVal[22];
  bool hidDVal[19]={0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1};
  byte hidRVal[3];
  long hidEVal[8];
  long hidEVal4[8];
  float lastHidAVal[22]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  bool lastHidDVal[19]={0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,1,1,1,1,1};
  long lastHidRVal[3];
  long lastHidEVal[8];
  long lastHidEVal4[8];

//fretboard
  byte frtb_sensMode=0;
  byte frtPins[] = {14,13,2,3,4,5,6,7,8,9,10,11,12,24,25,26,27,28,29,30,15,16};
  byte strPins[] = {18,19,20,21,22,23};
  bool frtState[nFrets][nStrings];
  bool lastFrtState[nFrets][nStrings];
  byte strPrs[nStrings]={0,0,0,0,0,0};
  byte lastStrPrs[nStrings]={0,0,0,0,0,0};
  byte lastLastStrPrs[nStrings]={0,0,0,0,0,0};
  int strBnc[nStrings];
  bool fbrdMode=0;

//kickup
  const byte kickupPins[6]={39,38,37,36,35,34};//34
  unsigned int kickDur[nStrings]={5,5,5,5,4,4};
  bool kickCue[nStrings]={0,0,0,0,0,0};
  bool kickState[nStrings];
  unsigned long kickTimer[nStrings];
  unsigned long kCueTimer; 
  unsigned int kCueInt=3;
  byte lastKickMode=0;
  byte kickSeq=0;
  bool kickOn;

//mulltiBow
  byte bowMode=2;
  //bool bowOn=0;
  byte lastBowMode=0;

// Scales
  const int nScales=11;
  const char* sclNm[nScales]={"off", "root", "pentatonic", "major", "minor", "hrm minor", "mel minor", "altered", "whole", "wholeHalf","chromatic"};
  float scls_sclPix[nStrings][nLedFrets][3];
  float scls_midiPix[nStrings][nLedFrets][3];
  int scls_sclClr=0;
  int scls_numSclStp[nScales]= {0, 1, 5, 7, 7, 7, 7, 7, 6, 8, 12};
  byte scls_sclSel=2;
  byte scls_sclStp=0;
  int scls_opMode=2;
  //int scls_nDispEncFnc=4; 
  
//string arpeggiator/sequencer
  byte strArp_opMode=2;
  byte strArp_act=0;
  float sclArpMode=7;
  float sclArpClkMode=4;
  float sclArpRpt=4;
  byte strArp_stp[nStrings][nFrets];
  long strArp_clk[nStrings];
  const int strArp_maxVisSteps = 16;
  const int strArp_maxSteps = 64;
  float strArp_gridPix[nStrings][strArp_maxVisSteps][3];
  float strArp_crsrPix[nStrings][strArp_maxVisSteps][3];
  float strArp_stpPix[nStrings][strArp_maxVisSteps][3];
  byte strArp_nStps[nStrings];
  byte strArp_tmDv[nStrings]={6,6,6,6,6,6};
  byte strArp_tmDvs[12]={96,64,48,32,24,16,12,8,6,4,3,2};
  const char* strArp_tmDvNm[12]={"1","1.5","2","3","4","6","8","12","16","24","32","64"};
  byte strArp_tmDvSel[nStrings]={8,8,8,8,8,8};
  byte strArp_nTmDvs=12;
  byte strArp_nxtClkFil[nStrings];
  byte strArp_nRpt[nStrings] = {1,1,1,1,1,1};

  const char* strArp_strPrsNm[]={"serial", "parallel"}; 
  const byte strArp_nStrPrsFnc=2;
  byte strArp_strPrsFnc=0;
  
  const char* strArp_strEncNm[]={"steps", "tmDv"}; 
  const byte strArp_nStrEncFnc=2;
  byte strArp_strEncFnc=1;
  
  const char* strArp_strBtnNm[]={"mute", "randomise"};
  const byte strArp_nStrBtnFnc=2;
  byte strArp_strBtnFnc=0;

//generic Sequenzer
  //globals
  float pttr_gridPix[nStrings][nLedFrets][3];
  int genSq_opMode=3;
  const int genSq_maxVisSteps = 16;
  const int genSq_maxSteps = 16;
  const int genSq_nPttn = 8;
  const int genSq_nSngs = 12;
  const int genSq_pttnMOff = genSq_maxVisSteps+3;
  

  const int genSq_nInst = 3;
  int genSq_actInst = 0;
  int genSq_actSng = 0;
  const char* genSq_SongNm[12]={"Song01","Song02","Song03","Song04","Song05","Song06","Song07","Song08","Song09","Song10","Song11","Song12"};
  const int genSq_nActPttns=12;
  int genSq_actPttns[genSq_nActPttns][genSq_nInst];
  int genSq_actPttnsIdx;

  float genSq_gridPix[nStrings][genSq_maxVisSteps][3];
  float genSq_crsrPix[nStrings][genSq_maxVisSteps][3];
  float genSq_stpPix[nStrings][genSq_maxVisSteps][3];
  
  float genSq_pttnGridPix[nStrings][genSq_nPttn/2][3];
  float genSq_pttnPttnPix[nStrings][genSq_nPttn/2][3];

  const int genSq_nTmDvs=13; //global
  int genSq_tmDvs[genSq_nTmDvs]={384,192,96,64,48,32,24,16,12,8,6,4,3};
  const char* genSq_tmDvNm[genSq_nTmDvs]={"4","2","1",".75","/2","/3","/4","/6","/8","/12","/16","/24","/32"};

  const char* genSq_strPrsNm[]={"pStp","oct","vel","dec","len","smp"}; 
  const int genSq_strPrsFnc_sStp=0;
  const int genSq_strPrsFnc_oct=1;
  const int genSq_strPrsFnc_vel=2;
  const int genSq_strPrsFnc_cc1=3;
  const int genSq_strPrsFnc_cc2=4;
  const int genSq_strPrsFnc_cc3=5;
  const int genSq_nStrPrsFnc=6;
  const int genSq_nCc=3;
  const int genSq_SelChnCC=54;
  const int genSq_maxStpV[genSq_nStrPrsFnc]={12,9,50,99,99,99};
  int genSq_strPrsFnc=0;
  int genSq_ccMp[genSq_nCc]={48,41,50};

  const char* genSq_strEncNm[]={"stps", "tmDv", "chn"}; 
  const int genSq_strEncFnc_stps=0;
  const int genSq_strEncFnc_tmDv=1;
  const int genSq_strEncFnc_chn=2;
  const int genSq_nStrEncFnc=3;
  const int genSeq_maxEncV[genSq_nStrPrsFnc]={genSq_maxVisSteps,genSq_nTmDvs,16};
  int genSq_strEncFnc=0;
  int genSq_strEncChAStps=0;
  
  const char* genSq_strBtnNm[]={"mute", "rnd", "sclQ"};
  const int genSq_strBtnFnc_mute=0;
  const int genSq_strBtnFnc_rnd=1;
  const int genSq_strBtnFnc_sclQ=2;
  const int genSq_nStrBtnFnc=3;
  int genSq_strBtnFnc=0;

  int genSq_stpEdtStr;
  bool genSq_stpEdtStrs[nStrings];
  int genSq_stpEdtFrt;

  float genSq_actPttnColor[3]={0.0,0.1,0.0};
  float genSq_edtPttnColor[3]={0.0,0.0,0.1};

  //per instance 
  int genSq_actNotes[genSq_nInst][nStrings][128];
  int genSq_velState[genSq_nInst][nStrings];
  int genSq_lastNote[genSq_nInst][nStrings];
  int genSq_clk[genSq_nInst][nStrings];
  int genSq_nxtClkFil[genSq_nInst][nStrings];
  int genSq_muteCh[genSq_nInst][nStrings];
  int genSq_actPttn[genSq_nInst];
  int genSq_edtPttn[genSq_nInst];
  //bool genSq_attSng[genSq_nInst]={1,0,0}; //depreceated
  bool genSq_sclQ[genSq_nInst][nStrings];
  //int genSq_sndCh[genSq_nInst][nStrings]={{1,1,1,1,1,1},{2,2,2,2,2,2},{3,3,3,3,3,3}};
  float genSq_gridColor[genSq_nInst][3]={{0.0,0.01,0.01},{0.015,0.0,0.01},{0.015,0.01,0.0}};

  // per pattern
  int genSq_tmDv[genSq_nInst][genSq_nPttn][nStrings];
  bool genSq_stpOnOff[genSq_nInst][genSq_nPttn][nStrings][genSq_maxSteps];
  int genSq_stp[genSq_nInst][genSq_nPttn][nStrings][genSq_maxSteps][genSq_nStrPrsFnc];
  int genSq_chn[genSq_nInst][genSq_nPttn][nStrings][genSq_nStrEncFnc]; 
 
// midi Instrument
  int mInst_chn=11;
  int mInst_anaXyCc[2]={1,2};
  int mInst_potCc[4]={3,4,5,6};

//Scale settings
  byte rootNote=0;

//sequence settings
  unsigned int bpm=90;

//string setup
 const char* toneNm[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

//display
  AsciiMassagePacker msg_disp;  //to drive the display
  unsigned long disp_frameTimer; //timer for the led update
  unsigned int disp_frameInt=200;

//global parameters
  const byte maxOpMds=12;
  byte opMode=2;
  byte opMdMap[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
  byte opStrMode=1;
  byte fledMode=opMode;
  bool shift=0;
  bool extClk=0;
  int dispEncMode;
  int strEncMode;
  int lastStrEnc;
  const byte nDispEncFnc[maxOpMds]={2,3,5,3,3,3,3,3,3,3,3,3}; //number of functions selectable with the disp encoder in each opMode
  const byte nStrEncFnc[maxOpMds]={2,2,2,2,2,2,2,2,2,2,2,2}; //number of functions selectable with the string encoders in each opMode
  int dispEncFnc[maxOpMds];
  byte strEncFnc[maxOpMds];
  float vol;
  

// Clock
  long mClock; //master clock
  long lastMClock; //master clock
  long pulse;
  long lastPulse;
  bool schdSync;
  unsigned int intClockInt; //inerval between clock ticks
  unsigned int intClockTimer; //to measure interval between clock ticks
  bool clckOn=0;

// midi 
  int midiCh=15;
  int extNotes[17][128];

//other
long scanPttnTimer;

void setup() {
  midiStop();
  Serial.begin(115200); //debug
  Serial1.begin(250000); //audio Server
  Serial4.begin(115200); // hid
  //Serial5.begin(250000); // LED
  MIDI.begin(MIDI_CHANNEL_OMNI); 

  //init sdCard
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // init LEDs  
  LEDS.addLeds<WS2812SERIAL,LED_PIN,RGB>(frtPix,NUMPIXELS);
  LEDS.setBrightness(byte(fled_bright*255));

  //kickup
  for(int s=0;s<nStrings;s++){ 
    pinMode(kickupPins[s],OUTPUT);
    digitalWrite(kickupPins[s], LOW);
  }

  //fretboard
  for (int s=0; s < nStrings; s++) {
    pinMode(strPins[s], INPUT_PULLUP);
  }
  for (int f=0; f < nFrets; f++) {
    pinMode(frtPins[f], OUTPUT);
    digitalWrite(frtPins[f], LOW);
  }

//set default genSq-parameter
  for (int i=0; i < genSq_nInst; i++) {
    genSq_actPttn[i]=0;
    genSq_edtPttn[i]=0;
    for (int p=0; p < genSq_nPttn; p++) {
      for (int s=0; s < nStrings; s++) {
        genSq_chn[i][p][s][genSq_strEncFnc_tmDv]=8; 
        genSq_chn[i][p][s][genSq_strEncFnc_stps]=16;
        genSq_clk[i][s]=-1;
        for (int f=0; f < genSq_maxSteps; f++) {
          genSq_stpOnOff[i][p][s][f]=0;
          genSq_stp[i][p][s][f][genSq_strPrsFnc_sStp]=0;
          genSq_stp[i][p][s][f][genSq_strPrsFnc_oct]=4;
          genSq_stp[i][p][s][f][genSq_strPrsFnc_vel]=40; 
          genSq_stp[i][p][s][f][genSq_strPrsFnc_cc1]=0; 
          genSq_stp[i][p][s][f][genSq_strPrsFnc_cc2]=0; 
          genSq_stp[i][p][s][f][genSq_strPrsFnc_cc3]=0;     
          genSq_tmDv[i][p][s]=6;
        }
      }
    }
  }
  loadSong(0);
  chngBpm(bpm);
  strArp_drwGrid();
  mkColors();
  setMidiHandles();
  clck_strt();
}

void loop() {
  //while (usbMIDI.read()){};
  MIDI.read();    
  
  updIntClock();
  
  if (fbrdMode==1)readFretboard(0);
  if (fbrdMode == 0)readFretboard(frtb_sensMode);
  
  cueKicks();
  updLedFrets();
  scanPttns();
  //updDisplay();
}
