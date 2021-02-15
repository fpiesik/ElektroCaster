void setup(void){
  
  Serial.begin(115200);
  Serial5.begin(250000);

  delay(1500);

  AudioMemory(1200);

  audioShield.enable();
  audioShield.volume(1);

//calculate midi frequencies
//for (int x = 0; x < 127; ++x){ 
//  midiFreq[x] = (tP / 32) * (2 ^ ((x - 9) / 12));
//}

//  nFreq6.begin(.15);
  nFreq2.begin(.15);

  for (int i=0; i<nStrings; i++) {
    nFreq[i]->begin(0.05);
  }

  for (int i=0; i<nStrings; i++) {
    nFreq[i]->stop();
  }
  
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
    cAmps[i]->gain(0.0001);
  }

  for (int i=0; i<nStrings; i++) {
    aBiasM[i]->gain(0,0.5);
    aBiasM[i]->gain(1,0.5 );
    fBiasM[i]->gain(0,1);
    fBiasM[i]->gain(1,0);
  }

  for (int i=0; i<nStrings; i++) {
    inGains[i]->gain(strGain[i]);
  }

  for (int i=0; i<nStrings; i++) {
    preA[i]->gain(2);
  }

  
  dcFEnv.amplitude(1);
  ampOut.gain(1);

not0.begin(WAVEFORM_SINE);
not0.frequency(1);
not0.amplitude(0.0001);

float shpStr[9]= {
  -1,
  -0.95,
  -0.9,
  -0.85,
  0,
  0.85,
  0.9,
  0.95,
  1,
};

  for (int i=0; i<nStrings; i++) {
    ws[i]->shape(shpStr,9);
  }
float shp[9]={-1,-0.95,-0.9,-0.85,0,0.85,0.9,0.95,1};
waveshaper1.shape(shp,9);
waveshaper2.shape(shp,9);
waveshaper3.shape(shp,9);
waveshaper4.shape(shp,9);
waveshaper5.shape(shp,9);
waveshaper6.shape(shp,9);


  for (int i=0; i<nStrings; i++) {
    aEnvs[i]->delay(envPA[0]);
    aEnvs[i]->attack(envPA[1]);
    aEnvs[i]->hold(envPA[2]);
    aEnvs[i]->decay(envPA[3]);
    aEnvs[i]->sustain(0);
    aEnvs[i]->release(envPA[5]);
  }

    for (int i=0; i<nStrings; i++) {
    fEnvs[i]->delay(envPF[0]);
    fEnvs[i]->attack(envPF[1]);
    fEnvs[i]->hold(envPF[2]);
    fEnvs[i]->decay(envPF[3]);
    fEnvs[i]->sustain(0);
    fEnvs[i]->release(envPF[5]);
  }

outMix1.gain(3,0.001);
outMix2.gain(3,0.001);
outMix3.gain(3,0.001);
outMix4.gain(3,0.001);
outMix5.gain(3,0.001);
outMix6.gain(3,0.001);
outMix7.gain(3,0.001);
outMix8.gain(3,0.001);


//  delay0.begin(tape_delay_bank, DELAY_MAX_LEN, 5000, 1, 2); //bank to use, size of bank, delay time in samples , rate reduction, interpolation time
  
  opmode=0;
  

usbMIDI.setHandleControlChange(rcvCC);
usbMIDI.setHandleClock(midiClock);
usbMIDI.setHandleStart(midiStart);
//usbMIDI.setHandleContinue(midiContinue);
usbMIDI.setHandleStop(midiStop);
usbMIDI.setHandleNoteOn(rcvNoteOn);
usbMIDI.setHandleNoteOff(rcvNoteOff);

  Serial.print(AudioProcessorUsage());
delay(2000);
sndFin();

}
