#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioControlCS42448 audioShield;

// GUItool: begin automatically generated code
AudioInputTDM            tdm1;           //xy=335,320
AudioMixer4              mixer2;         //xy=569,330
AudioMixer4              mixer1;         //xy=570,239
AudioMixer4              mixer3;         //xy=770,257
AudioOutputTDM           tdm2;           //xy=942,322
AudioConnection          patchCord1(tdm1, 0, mixer1, 0);
AudioConnection          patchCord2(tdm1, 2, mixer1, 1);
AudioConnection          patchCord3(tdm1, 4, mixer1, 2);
AudioConnection          patchCord4(tdm1, 6, mixer1, 3);
AudioConnection          patchCord5(tdm1, 8, mixer2, 0);
AudioConnection          patchCord6(tdm1, 10, mixer2, 1);
//AudioConnection          patchCord7(tdm1, 12, mixer2, 2);
//AudioConnection          patchCord8(tdm1, 14, mixer2, 3);
AudioConnection          patchCord9(mixer2, 0, mixer3, 1);
AudioConnection          patchCord10(mixer1, 0, mixer3, 0);
AudioConnection          patchCord11(mixer3, 0, tdm2, 12);
// GUItool: end automatically generated code



void setup(void){
  AudioMemory(200);
  audioShield.enable();
  audioShield.volume(1);
//  audioShield.inputLevel(1,0.1);
//  audioShield.inputLevel(2,0.1);
//  audioShield.inputLevel(3,0.1);
//  audioShield.inputLevel(4,0.1);
//  audioShield.inputLevel(5,0.1);
//  audioShield.inputLevel(6,0.1);
  audioShield.inputLevel(0.1);
  //audioShield.inputSelect(0);

//  waveform1.frequency(440);
//  waveform1.amplitude(0.5);
//  waveform1.begin(WAVEFORM_SINE);
//
//  amp1.gain(10);

}

void loop(){
  
}
