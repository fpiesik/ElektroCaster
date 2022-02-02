#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
#include <Wire.h>
#include <EEPROM.h>
#include <AsciiMassagePacker.h>

//string defintions variables
  const int nStrings=6; //how many Strings
  const int nFrets=22;
  byte tuning[nStrings]={64,59,55,50,45,40};
  const byte strSnsPins[nStrings]={2,3,4,5,6,7};
  unsigned long lastFretRead[nStrings];
  unsigned int fretMaskT=70; //time until a next strPres on he same string is detected
  unsigned int strBncs=100; //number of same samples to trigger strPres
  float strP[nStrings];
  float lastStrP[nStrings];
  float strA[nStrings];
  float lastStrA[nStrings];

//led defintions and variables
  #define LED_PIN     33   //led pin
  #define NUMPIXELS    150  //total number of leds
  const int nLedFrets=25; //how many led frets
  float tnClrs[12][3];
  float fled_bright = 0.6; //brightness
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
  byte frtPins[] = {14,13,2,3,4,5,6,7,8,9,10,11,12,24,25,26,27,28,29,30,16,15};
  byte strPins[] = {18,19,20,21,22,23};
  bool frtState[nFrets][nStrings];
  bool lastFrtState[nFrets][nStrings];
  byte strPrs[nStrings]={0,0,0,0,0,0};
  byte lastStrPrs[nStrings]={0,0,0,0,0,0};
  byte lastLastStrPrs[nStrings]={0,0,0,0,0,0};
  int strBnc[nStrings];

//kickup
  const byte kickupPins[6]={39,38,37,36,35,34};
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
  const int nScales=8;
  const char* sclNm[nScales]={"off", "root","major","pentatonic", "mixob9b13","whole tone","wholeHalf","altered"};
  float scls_sclPix[nStrings][nLedFrets][3];
  float scls_midiPix[nStrings][nLedFrets][3];
  int scls_sclClr=1;
  int scls_numSclStp[nScales]= {0, 1, 7, 5, 7, 6, 8, 7};
  byte scls_sclSel=2;
  byte scls_sclStp=0;
  
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

  
//Drum Sequenzer
  const int drmSq_maxVisSteps = 16;
  const int drmSq_maxSteps = 64;
  byte drmSq_nSteps[nStrings] = {16,16,16,16,16,16};
  float drmSq_gridPix[nStrings][drmSq_maxVisSteps][3];
  float drmSq_crsrPix[nStrings][drmSq_maxVisSteps][3];
  float drmSq_stpPix[nStrings][drmSq_maxVisSteps][3];
  byte drmSq_stp[nStrings][drmSq_maxSteps][1];
  byte drmSq_velState[nStrings];
  long drmSq_clk[nStrings];
  byte drmSq_opMode=3;
  byte drmSq_tmDv[nStrings]={6,6,6,6,6,6};
  byte drmSq_tmDvs[12]={96,64,48,32,24,16,12,8,6,4,3,2};
  const char* drmSq_tmDvNm[12]={"1","1.5","2","3","4","6","8","12","16","24","32","64"};
  byte drmSq_tmDvSel[nStrings]={8,8,8,8,8,8};
  byte drmSq_nxtClkFil[nStrings];
  byte drmSq_nTmDvs=12;
  byte drmSq_muteCh[nStrings];
  //const byte drmSq_nStrEncFnc=2;

  const char* drmSq_strPrsNm[]={"placeStp", "velocity", "prob", "stepJmp"}; 
  const byte drmSq_nStrPrsFnc=4;
  byte drmSq_strPrsFnc=0;
  
  const char* drmSq_strEncNm[]={"steps", "tmDv"}; 
  const byte drmSq_nStrEncFnc=2;
  byte drmSq_strEncFnc=1;
  
  const char* drmSq_strBtnNm[]={"mute", "randomise"};
  const byte drmSq_nStrBtnFnc=2;
  byte drmSq_strBtnFnc=0;

//Pattern settings
  byte rootNote=0;
  int actScale=0;

//sequence settings
  unsigned int bpm=90;

//string setup
  char* toneNm[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

//display
  AsciiMassagePacker msg_disp;  //to drive the display
  unsigned long disp_frameTimer; //timer for the led update
  unsigned int disp_frameInt=200;

//global parameters
  const byte maxOpMds=12;
  byte opMode=1;
  byte opMdMap[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
  byte opStrMode=1;
  byte fledMode=opMode;
  bool shift=0;
  bool extClk=0;
  byte dispEncMode;
  const byte nDispEncFnc[maxOpMds]={2,4,3,3,2,2,2,2,2,2,2,2}; //number of functions selectable with the disp encoder in each opMode
  const byte nStrEncFnc[maxOpMds]={2,2,2,2,2,2,2,2,2,2,2,2}; //number of functions selectable with the disp encoder in each opMode
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

// midi 
  int midiCh=15;
  int extNotes[127];

void setup() {
  for(int i=0; i<127; i++){
    extNotes[i]=0;
  }
  Serial.begin(115200); //debug
  Serial1.begin(250000); //audio Server
  Serial4.begin(115200); // hid
  
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
  
  chngBpm(bpm);
  drmSq_drwGrid();
  strArp_drwGrid();
  mkColors();

  setMidiHandles();
}

void loop() {
  usbMIDI.read();
  updIntClock();
  if (opStrMode == 3 )readFretboard(0);
  if (opStrMode == 1 || opStrMode == 2){
    if(shift==0)readFretboard(frtb_sensMode);
  }
  cueKicks();
  updLedFrets();
  updDisplay();

}
