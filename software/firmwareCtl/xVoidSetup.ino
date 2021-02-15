void setup() {
  delay(2000);
  LEDS.addLeds<WS2812SERIAL,DATA_PIN,RGB>(frtPix,NUMPIXELS);
  LEDS.setBrightness(byte(bright*255));
  
  Serial.begin(115200);
  Serial1.begin(250000);
  Serial4.begin(115200);

//setup display
//  u8g2.begin();
//  delay (1000);
//  u8g2.clearBuffer();
//  draw();
//  u8g2.sendBuffer();

//setup kickup
  for(int s=0;s<nStrings;s++){ 
    pinMode(kickupPins[s],OUTPUT);
    digitalWrite(kickupPins[s], LOW);
  }

  for(int s=0;s<nStrings;s++){ 
    pinMode(strSnsPins[s],INPUT_PULLUP);
  }


  mkColors();
  mkColorsHd();
  
  hidEVal[7]=90;
  hidEVal4[7]=hidEVal[7]*4;

  for(int s=0;s<nStrings;s++){ 
  for(int f=0;f<nLedFrets;f++){ 
  }}

  //cTimer.begin(intClock, 20000);
}
