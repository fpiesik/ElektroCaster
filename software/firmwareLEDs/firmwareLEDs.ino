//#include <WS2812Serial.h>
//#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

//led defintions and variables
  #define LED_PIN     6   //led pin
  #define NUMPIXELS    150  //total number of leds
  Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
  const int nStrings = 6;
  const int nLedFrets=25;

  const byte led_pixPos[nStrings][nLedFrets]={
  {144,143,132,131,120,119,108,107,96,95,84,83,72,71,60,59,48,47,36,35,24,23,12,11,0}, //for 25 frets
  {145,142,133,130,121,118,109,106,97,94,85,82,73,70,61,58,49,46,37,34,25,22,13,10,1}, //for 25 frets
  {146,141,134,129,122,117,110,105,98,93,86,81,74,69,62,57,50,45,38,33,26,21,14,9,2}, //for 25 frets
  {147,140,135,128,123,116,111,104,99,92,87,80,75,68,63,56,51,44,39,32,27,20,15,8,3}, //for 25 frets
  {148,139,136,127,124,115,112,103,100,91,88,79,76,67,64,55,52,43,40,31,28,19,16,7,4}, //for 25 frets
  {149,138,137,126,125,114,113,102,101,90,89,78,77,66,65,54,53,42,41,30,29,18,17,6,5}}; //for 25 frets

void setup() {
  Serial.begin(250000);
  // init LEDs  
  pixels.begin();
  //LEDS.addLeds<WS2812SERIAL,LED_PIN,RGB>(frtPix,NUMPIXELS);
  //LEDS.setBrightness(byte(fled_bright*255));

}

void loop() {
  // put your main code here, to run repeatedly:

}


void pixel(byte s, byte f, byte c[3]){
  pixels.setPixelColor(led_pixPos[s][f], pixels.Color(c[0], c[1], c[2]));
  //frtPix[led_pixPos[s][f]]=CRGB(c[2]*fled_blueC, c[0]*fled_redC, c[1]*fled_greenC);
}
