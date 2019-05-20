#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Encoder.h>

#include <AsciiMassageParser.h>
 AsciiMassageParser inbound;
#include <AsciiMassagePacker.h>
AsciiMassagePacker outbound;

Encoder enc0(16, 15);
Encoder enc1(3, 2);
Encoder enc2(6, 5);
Encoder enc3(9, 8);
Encoder enc4(12, 11);
Encoder enc5(26, 24);
Encoder enc6(29,28);
Encoder enc7(45, 44);



U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const int nAnalog = 22;
const int nDigital = 19;
const int nEnc = 7;

const int aPin[nAnalog] = {31, 32, 33, 34, 35, 36, 37, 38, 39, A21, A22, 14, A26, A25, A11, A10, A24, A23, A9, A8, A7, A6 };

const int dPin[nDigital] = {17, 13, 51, 52, 53, 42, 41, 40, 56, 57, 43, 54, 55, 4, 7, 10, 25, 27, 30};

int aVal[nAnalog];
int lastAVal[nAnalog];
//int aSmpl=20;

bool dVal[nDigital];
bool lastDVal[nDigital];

long pos0 = 0;
long pos1 = 0;
long pos2 = 0;
long pos3 = 0;
long pos4 = 0;
long pos5 = 0;
long pos6 = 0;
long pos7 = 0;

long newEnc0,newEnc1,newEnc2,newEnc3,newEnc4,newEnc5,newEnc6,newEnc7;

int rsVal[12]={341,231,163,93,960,916,855,771,679,608,507,404};
int rsWin=20;
byte rsState[3]={0,0,0};
byte lastRsState[3]={255,255,255};
int waitS=50;

void setup() {
  Serial.begin(115200);
  Serial6.begin(115200);
  for (int i = 0; i < nDigital; i++) {
    pinMode(dPin[i], INPUT);
    digitalWrite(dPin[i], HIGH);
  }
  analogReadAveraging(200);
  //analogReadResolution(12);
u8g2.begin();
delay (2000);
u8g2.clearBuffer();
draw();
u8g2.sendBuffer();
sendAll();
}

void loop() {
  for (int i = 0; i < nDigital; i++) {
    dVal[i] = digitalRead(dPin[i]);
  }
  
  for (int i = 0; i < nAnalog; i++) {
    aVal[i]=analogRead(aPin[i]);
    }

  newEnc0 = enc0.read();
  newEnc1 = enc1.read();
  newEnc2 = enc2.read();
  newEnc3 = enc3.read();
  newEnc4 = enc4.read();
  newEnc5 = enc5.read();
  newEnc6 = enc6.read();
  newEnc7 = enc7.read();

  for(int r=19;r<=21;r++){
    if(aVal[r]>30){
      int val=aVal[r];
      for(int i=0;i<12;i++){
        if(val<rsVal[i]+rsWin&&val>rsVal[i]-rsWin)rsState[r-19]=i;
      }
    }
  }
  sendAllNew();

}
