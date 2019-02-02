#include <Adafruit_NeoPixel.h>
#include <OSCMessage.h>
#include <DueTimer.h>
#ifdef __AVR__
//#include <avr/power.h>
#endif
#define PIN            6
#define NUMPIXELS    102

#include <AsciiMassageParser.h>
 AsciiMassageParser inbound;
#include <AsciiMassagePacker.h>
AsciiMassagePacker outbound;
AsciiMassagePacker outLed;

//constants------------------------------------- 
const int nStrings=6; //how many Strings
const int nLedFrets=17; //how many led frets

byte pixPos[nStrings][nLedFrets]={{96,95,84,83,72,71,60,59,48,47,36,35,24,23,12,11,0},{97,94,85,82,73,70,61,58,49,46,37,34,25,22,13,10,1},
{98,93,86,81,74,69,62,57,50,45,38,33,26,21,14,9,2},{99,92,87,80,75,68,63,56,51,44,39,32,27,20,15,8,3},{100,91,88,79,76,67,64,55,52,43,40,31,28,19,16,7,4},
{101,90,89,78,77,66,65,54,53,42,41,30,29,18,17,6,5}};

//parameters----------------------------------
byte tuning[nStrings]={59,54,50,45,40,35};
byte rootNote=0 ;
int trans=11;
int actScale=0;

byte ccArpMode=28;
byte ccArpClkMode=29;
byte ccArpRpt[7]={20,21,22,23,24,25,26};
float sclArpMode=7;
float sclArpClkMode=4;
float sclArpRpt=4;
int arpSize=0;
byte arpSeq[64];

byte ccTd[7]={10,11,12,13,14,15,16};
float sclTd=7;
byte tmDv[nStrings]={6,6,6,6,6,6};
byte tmDvs[12]={96,64,48,32,24,16,12,8,6,4,3,2};

int rsVal[12]={341,231,163,93,960,916,855,771,679,608,507,404};
int rsWin=20;
byte rsState[3]={0,0,0};
byte lastRsState[3]={255,255,255};

float envP[6]={60,150,0,300,0.3,299};
float envPMax[6]={100,300,300,300,1.0,300};
byte cEnvP[6];


int waitS=500; //wait after serial send

//ledFrets
float bright = 1; //brightness
float redC = 35;  //compensate the "wood filament filter"
float greenC= 100; //compensate the "wood filament filter"
float blueC = 255; //compensate the "wood filament filter"

float trgtC[nStrings][nLedFrets][3];
float actC[nStrings][nLedFrets][3];

//kickup
int kickDur[nStrings]={8000,8000,8000,8000,8000,8000};

//pin assignments
const byte kickupPins[6]={32,30,28,26,24,22};
const byte potPins[2]={A11,A10};
const byte sw1Pins[2]={2,3};


//variables
float potState[2]={0,0};

long frmCnt=0;
long lastFrmCnt=0;

unsigned long intClockTimer;
unsigned int bpm=30;

unsigned long frameTimer; //timer for the led update
unsigned int frameInt=50;

unsigned long ctlTimer; //timer for control actions
unsigned int ctlInt=10;

unsigned long mClock=0; //master clock with the max resolution
unsigned long lastMClock=0;

unsigned long tdClock[nStrings]={0,0,0,0,0,0}; //actual clock depends on the time devision
unsigned long lastTdClock[nStrings]={0,0,0,0,0,0};

byte seqIdx[nStrings]; //index of the seq/arp (different than the tdClock?)
byte lastSeqIdx[nStrings];

bool stepState[nStrings][nLedFrets];
byte clockMode=1;
byte lastClockMode=0;
byte opmode=0;
byte lastOpmode=0;
byte dispMode=0;
byte lastDispMode=0;
byte arpMode=0;
byte arpClkMode=0;
byte arpRpt[6]={1,1,1,1,1,1};
byte pOrder[1000]; // order in which the strings were pressed
int pOrderIdx=0;

byte kickMode=3;
bool kickOn=0;

byte lastKickMode=0;
byte bowMode=2;
bool bowOn=0;
byte lastBowMode=0;

byte editMode=0;
byte lastEditMode=0;

float midiNotes[128];
float midiTones[128];

float hsbRGB[3]={0,0,0};

static byte lastRowUsed[nStrings];
byte seqNStp[nStrings]={16,16,16,16,16,16};
byte seqSpd[nStrings]={6,6,6,6,6,6};

float hidAVal[22];
bool hidDVal[19];
byte hidRVal[3];
long hidEVal[8];

float lastHidAVal[22];
bool lastHidDVal[19];
long lastHidRVal[3];
long lastHidEVal[8];

bool shift=0;
bool play=1;
byte gotOsc=0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
