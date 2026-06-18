#include "../shared/ProtocolAudio.h"
#include "../shared/ProtocolHid.h"

void serialEvent1(){
  int incoming=-1;
  byte sbyte = Serial1.read();
  if (sbyte >= AUDIO_CMD_BASE && sbyte <= AUDIO_CMD_MAX) incoming = audioIncoming(sbyte);
    if (incoming == audioIncoming(AUDIO_STATUS_STR_PITCH)){
    byte a;
    byte b;
    byte c;
    while(Serial1.available() == 0);
    a=Serial1.read();
    while(Serial1.available() == 0);
    b=Serial1.read();
    while(Serial1.available() == 0);
    c=Serial1.read();
    strP[a]=b+c/100.0;
  }
    if (incoming == audioIncoming(AUDIO_STATUS_STR_AMPLITUDE)){
    byte a;
    byte b;
    while(Serial1.available() == 0);
    a=Serial1.read();
    while(Serial1.available() == 0);
    b=Serial1.read();
    strA[a]=b/100.0;
  }  
}

void serialEvent7(){   
   
  int incoming=-1;
  byte serbyte = Serial7.read();
  if (serbyte >= HID_CMD_BASE && serbyte <= HID_CMD_MAX) incoming = hidIncoming(serbyte);
  //if (serbyte == 255)sndGetMidi();

  if(incoming>=0){
    while(Serial7.available() == 0);
    byte val=Serial7.read();
    if (incoming < HID_ANALOG_OFFSET) rcvHidD(incoming,val),incoming=-1;
    if (incoming >= HID_ANALOG_OFFSET && incoming < HID_ROTARY_OFFSET) rcvHidA(incoming - HID_ANALOG_OFFSET,val);
    if (incoming >= HID_ROTARY_OFFSET && incoming < HID_ENCODER_OFFSET) rcvHidR(incoming - HID_ROTARY_OFFSET,val);
    if (incoming >= HID_ENCODER_OFFSET && incoming < HID_ENCODER_END_OFFSET) rcvHidE(incoming - HID_ENCODER_OFFSET,val - HID_ENCODER_CENTER);
  }
}
