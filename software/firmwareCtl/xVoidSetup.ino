void setup() {

  LEDS.addLeds<WS2812SERIAL,DATA_PIN,RGB>(frtPix,NUMPIXELS);
  LEDS.setBrightness(byte(bright*255));
  
  Serial.begin(115200);
  Serial1.begin(250000);
  Serial4.begin(115200);




  for(int s=0;s<nStrings;s++){ 
    pinMode(kickupPins[s],OUTPUT);
    digitalWrite(kickupPins[s], LOW);
  }

  for(int s=0;s<nStrings;s++){ 
    pinMode(strSnsPins[s],INPUT_PULLUP);
  }


  mkColors();

  hidEVal[7]=90;
  hidEVal4[7]=hidEVal[7]*4;

  for(int s=0;s<nStrings;s++){ 
  for(int f=0;f<nLedFrets;f++){ 
  }}

  //cTimer.begin(intClock, 20000);
}
