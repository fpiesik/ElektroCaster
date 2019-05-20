void setup(void){
  
  Serial.begin(115200);
  Serial5.begin(500000);


  AudioMemory(2000);

  audioShield.enable();
  audioShield.volume(1);
  
  for (int i=0; i<nStrings; i++) {
    filter[i]->frequency(0);
    filter[i]->resonance(0.707);
    filter[i]->octaveControl(7);
  }

  for (int i=0; i<nStrings; i++) {
    lfos1[i]->begin(WAVEFORM_SINE);
    lfos1[i]->frequency(1);
    lfos1[i]->amplitude(1);
  }

  for (int i=0; i<nStrings; i++) {
    amps[i]->gain(0);
  }

  for (int i=0; i<nStrings; i++) {
    gains[i]->gain(strGain[i]);
  }

  dcEnv2.amplitude(1);
  ampOut.gain(1);


float shp[5]={-1,-0.5,0,0,0};
waveshaper1.shape(shp,5);
waveshaper2.shape(shp,5);
waveshaper3.shape(shp,5);
waveshaper4.shape(shp,5);
waveshaper5.shape(shp,5);
waveshaper6.shape(shp,5);


  for (int i=0; i<nStrings; i++) {
    envs[i]->delay(envP[0]);
    envs[i]->attack(envP[1]);
    envs[i]->hold(envP[2]);
    envs[i]->decay(envP[3]);
    envs[i]->sustain(envP[4]);
    envs[i]->release(envP[5]);
  }


  mixer3.gain(0,1.0);
  mixer3.gain(1,0.3);
  mixer4.gain(0,1.0);
  mixer4.gain(1,0.5);

  delay0.begin(tape_delay_bank, DELAY_MAX_LEN, 5000, 1, 2); //bank to use, size of bank, delay time in samples , rate reduction, interpolation time
  
  opmode=0;
  
  for (int i=0; i<nStrings; i++) {
    envs[i]->sustain(1);
    envs[i]->noteOn();
    envs2[i]->sustain(1);
    envs2[i]->noteOn();
  }


//usbMIDI.setHandleControlChange(rcvCC);
//usbMIDI.setHandleClock(midiClock);
//usbMIDI.setHandleStart(midiStart);
//usbMIDI.setHandleContinue(midiContinue);
//usbMIDI.setHandleStop(midiStop);
usbMIDI.setHandleNoteOn(rcvNoteOn);
usbMIDI.setHandleNoteOff(rcvNoteOff);

  Serial.print(AudioProcessorUsage());
delay(2000);
sndFin();

}
