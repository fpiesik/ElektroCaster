#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
//#include <U8g2lib.h>
#include <Wire.h>
//#include <Arduino.h>

#define DATA_PIN     33   //led pin
#define NUMPIXELS    150   

//#include <AsciiMassagePacker.h>
//AsciiMassagePacker outbound;  //to drive the display

CRGB frtPix[NUMPIXELS];

//display-------------------------------------------------------
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//constants------------------------------------- 
const int nStrings=6; //how many Strings
const int nLedFrets=25; //how many led frets

const byte pixPos[nStrings][nLedFrets]={
  {144,143,132,131,120,119,108,107,96,95,84,83,72,71,60,59,48,47,36,35,24,23,12,11,0},
  {145,142,133,130,121,118,109,106,97,94,85,82,73,70,61,58,49,46,37,34,25,22,13,10,1},
  {146,141,134,129,122,117,110,105,98,93,86,81,74,69,62,57,50,45,38,33,26,21,14,9,2},
  {147,140,135,128,123,116,111,104,99,92,87,80,75,68,63,56,51,44,39,32,27,20,15,8,3},
  {148,139,136,127,124,115,112,103,100,91,88,79,76,67,64,55,52,43,40,31,28,19,16,7,4},
  {149,138,137,126,125,114,113,102,101,90,89,78,77,66,65,54,53,42,41,30,29,18,17,6,5}};


//settings----------------------------------
byte tuning[nStrings]={64,59,55,50,45,40};
byte rootNote=0 ;
int actScale=0;
const int nScales=3;

// debug-----------------------------------------
int dbgLvl=0; //debug level
unsigned long debugTimer;
unsigned int debugInt=2000;

bool dispEncFnc=0;

//continious controller message assignments
byte ccArpMode=28;
byte ccArpClkMode=29;
byte ccArpRpt[7]={20,21,22,23,24,25,26};
byte ccOpMode=40;
byte ccTd[7]={30,31,32,33,34,35,36};
byte ccLghtHold=50;
byte ccLghtHoldRst=51;
byte ccNotesOrTones=60;

float sclArpMode=7;
float sclArpClkMode=4;
float sclArpRpt=4;
int arpSize=0;
byte arpSeq[64];


float sclTd=11;
byte tmDv[nStrings]={6,6,6,6,6,6};
byte tmDvs[12]={96,64,48,32,24,16,12,8,6,4,3,2};

float envP[6]={60,150,0,300,0.3,299};
float envPMax[6]={100,300,300,300,1.0,300};
byte cEnvP[6];

int waitS=50; //wait after each serial send

//ledFrets
float bright = 0.75; //brightness
float redC = 150;  //compensate the "wood filament filter"
float greenC= 255; //compensate the "wood filament filter"
float blueC = 255; //compensate the "wood filament filter"

float trgtC[nStrings][nLedFrets][3];
float actC[nStrings][nLedFrets][3];

//kickup
unsigned int kickDur[nStrings]={5,5,5,5,4,4};
bool kickCue[nStrings]={0,0,0,0,0,0};
bool kickState[nStrings];
unsigned long kickTimer[nStrings];
unsigned long kCueTimer; 
unsigned int kCueInt=2;

//pin assignments
const byte kickupPins[6]={39,38,37,36,35,34};//{23,22,21,20,19,18};
const byte strSnsPins[nStrings]={2,3,4,5,6,7};

//variables
long frmCnt=0;
long lastFrmCnt=0;

unsigned long intClockTimer;
unsigned int intClockInt=50;
unsigned int bpm=90;

unsigned long frameTimer; //timer for the led update
unsigned int frameInt=50;

unsigned long ctlTimer; //timer for control actions
unsigned int ctlInt=10;

long mClock=0; //master clock with the max resolution
unsigned long lastMClock=0;

unsigned long lastFretRead[nStrings];
unsigned int fretReadInt=50;

unsigned long tdClock[nStrings]={0,0,0,0,0,0}; //actual clock depends on the time devision
unsigned long lastTdClock[nStrings]={0,0,0,0,0,0};

int seqIdx[nStrings]; //index of the seq/arp (different than the tdClock?)
int lastSeqIdx[nStrings];

float strP[nStrings];
float lastStrP[nStrings];

long mClockDev;

float tnClrs[12][3];
float tnClrsHd[120][3];

bool stepState[nStrings][nLedFrets];
byte clockMode=1;
byte lastClockMode=0;
byte opmode=0;
byte lastOpmode=0;
byte dispMode=0;
byte nDispSrc=0;
byte frtGrid=1;
byte lastDispMode=0;
byte arpMode=0;
byte arpClkMode=0;
byte arpRpt[6]={1,1,1,1,1,1};
byte seqNDur[6]={4,4,4,4,4,4};
byte pOrder[1000]; // order in which the strings were pressed
int pOrderIdx=0;

byte kickMode=3;
bool kickOn=0;

bool arpMute[nStrings]={0,0,0,0,0,0}; 

byte lastKickMode=0;
byte kickSeq=0;
byte bowMode=2;
bool bowOn=0;
byte lastBowMode=0;

byte editMode=0;
byte lastEditMode=0;

bool NotesOrTones=0;

float midiNotes[128];
float lastMidiNotes[128];
float midiTones[12];
float lastMidiTones[12];


bool lghtHold=0;
bool lastLghtHold=0;
bool ntLghtHold[12]; // gather incoming Notes to display

float hsbRGB[3]={0,0,0};

static byte lastStrUsed[nStrings];
byte seqNStp[nStrings]={16,16,16,16,16,16};
byte seqSpd[nStrings]={6,6,6,6,6,6};

float hidAVal[22];
bool hidDVal[19]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte hidRVal[3];
long hidEVal[8];
long hidEVal4[8];


float lastHidAVal[22]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
bool lastHidDVal[19]={0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,0,0,0,0,0,0};
long lastHidRVal[3];
long lastHidEVal[8];
long lastHidEVal4[8];

bool shift=0 ;
bool runSeq=0;
bool lastRunSeq=0;
byte gotOsc=0;

byte incoming;
int serbyte;
