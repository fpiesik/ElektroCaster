void setup(void){
  
  Serial.begin(115200);
  Serial1.begin(250000);

  delay(1000);

  AudioMemory(2000);

  audioShield.enable();
  audioShield.volume(1);

  audioShield.inputLevel(0.5);
//  audioShield.inputLevel(2,4); // specific channel gain doesnt work
//  audioShield.inputLevel(3,4);
//  audioShield.inputLevel(4,4);
//  audioShield.inputLevel(5,4);
//  audioShield.inputLevel(6,4);

  for (int s=0; s<nStrings; s++){
    //mPC001[i] = new AudioConnection(dcFEnv,0, aBiasM[i], 0);
    //mPC001[i] = new AudioConnection(dcFEnv,0, fBiasM[i], 0);
    cbl_input[s] = new AudioConnection(audioInput, strAIn[s], inGain[s], 0); //connect the inputs with the input gain stage (audioInput, 10, inGain1, 0); strAIn
    cbl_peak[s] = new AudioConnection(inGain[s],0, strPeak[s], 0); //connect input gain stage to the peak detector  (inGain1, 0, peak1, 0);
    cbl_pitch[s] = new AudioConnection(inGain[s],0, nFreq[s], 0); // connect the input stage to the pitch detector (inGain1, 0, nFreq1, 0);
    cbl_ringAmpSig[s] = new AudioConnection(inGain[s],0, ringAmp[s], 0); //connect the input stage to the ringmod amp (inGain1, 0, mulA1, 0)
    cbl_ringAmpLFO[s] = new AudioConnection(lfo[s],0, ringAmp[s], 1); // connect the lfo to the ringmod amp (lfo1, 0, mulA1, 1);
    cbl_aBiasDryIn[s] = new AudioConnection(ringAmp[s],0, aBiasM[s], 0); // connect the ring Amp to the amp bias mixer (mulA1, 0, aBiasM1, 0);
    cbl_aBiasEnvIn[s] = new AudioConnection(aEnv[s],0, aBiasM[s], 1); // connect the amp envelope to the amp bias mixer (aEnv1, 0, aBiasM1, 1);
    cbl_aEnvRingIn[s] = new AudioConnection(ringAmp[s],0, aEnv[s], 0); // connect the ring amp to the amp envelope (mulA1, 0, aEnv1, 0);
    cbl_filterInSig[s] = new AudioConnection(aBiasM[s],0, filter[s], 0); //connect bias mixer to the filter audio input (aBiasM1, 0, filter1, 0);
    cbl_filterInCutoff[s] = new AudioConnection(fBiasM[s],0, filter[s], 1); //connect the filter bias mixer to filter cutoff input(fBiasM[0], 0, filter1, 1);
    cbl_fBiasInDC[s] = new AudioConnection(dcFEnv,0, fBiasM[s], 0); //connect dc to the filter bias mixer (dcFEnv,0, fBiasM[0], 0);
    cbl_fEnvInDC[s] = new AudioConnection(dcFEnv,0, fEnv[s], 0); //connect dc to the filter envelope to (dcFEnv, 0, fEnv1, 0);
    cbl_fBiasInFEnv[s] = new AudioConnection(fEnv[s],0, fBiasM[s], 1); //connect the filter envelope to the filter bias mixer (fEnv1, 0, fBiasM[0], 1);
    cbl_coilShaperIn[s] = new AudioConnection(inGain[s],0, coilShaper[s], 0); //connect the input stage to the coil waveshaper (inGain1, 0, waveshaper1, 0);
    cbl_coilAmpIn[s] = new AudioConnection(coilShaper[s],0, coilAmp[s], 0); //connect the coil waveshaper to the coil amp (waveshaper1, 0, cAmp1, 0);
    cbl_coilAmpOut[s] = new AudioConnection(coilAmp[s],0, audioOutput, strAOut[s]); //connect the coil amps to the outputs (coilAmp[0],0, audioOutput,10);
  }

//calculate midi frequencies
//for (int x = 0; x < 127; ++x){ 
//  midiFreq[x] = (tP / 32) * (2 ^ ((x - 9) / 12));
//}

//  nFreq6.begin(.15);
//  nFreq2.begin(.15);

  for (int i=0; i<nStrings; i++) {
    nFreq[i].begin(0.2);
  }

//  for (int i=0; i<nStrings; i++) {
//    nFreq[i].stop();
//  }

//  for (int i=0; i<nStrings; i++) {
//    
//    inputLpFilter[i]->setLowpass(0, inputHicut[i], 0.1);
//  }
  
  for (int i=0; i<nStrings; i++) {
    filter[i].frequency(0);
    filter[i].resonance(0.707);
    filter[i].octaveControl(7);
  }

  for (int i=0; i<nStrings; i++) {
    lfo[i].begin(WAVEFORM_SINE);
    lfo[i].frequency(30);
    lfo[i].amplitude(0.5);
    lfo[i].offset(1);
    chLfo1(1, 1,i);
  }

  for (int i=0; i<nStrings; i++) {
    bowOn=0;
    coilAmp[i].gain(0.000);
  }
  
  for (int i=0; i<nStrings; i++) {
    aBiasM[i].gain(0,0.5);
    aBiasM[i].gain(1,0.5 );
    fBiasM[i].gain(0,1);
    fBiasM[i].gain(1,0);
  }

  for (int i=0; i<nStrings; i++) {
    inGain[i].gain(strInGain[i]);
  }


//  strDel[0]->begin(sample_delay_line1, DELAY_MAX_LEN);
//  strDel[1]->begin(sample_delay_line2, DELAY_MAX_LEN);
//  strDel[2]->begin(sample_delay_line3, DELAY_MAX_LEN);
//  strDel[3]->begin(sample_delay_line4, DELAY_MAX_LEN);
//  strDel[4]->begin(sample_delay_line5, DELAY_MAX_LEN);
//  strDel[5]->begin(sample_delay_line6, DELAY_MAX_LEN);
  
  dcFEnv.amplitude(1);
  ampOut.gain(1);


float shp[9]={-1,-0.95,-0.9,-0.85,0,0.85,0.9,0.95,1};
  for (int i=0; i<nStrings; i++) {
    coilShaper[i].shape(shp,9);
  }


  for (int i=0; i<nStrings; i++) {
    aEnv[i].delay(envPA[0]);
    aEnv[i].attack(envPA[1]);
    aEnv[i].hold(envPA[2]);
    aEnv[i].decay(envPA[3]);
    aEnv[i].sustain(0);
    aEnv[i].release(envPA[5]);
  }

  for (int i=0; i<nStrings; i++) {
    fEnv[i].delay(envPF[0]);
    fEnv[i].attack(envPF[1]);
    fEnv[i].hold(envPF[2]);
    fEnv[i].decay(envPF[3]);
    fEnv[i].sustain(0);
    fEnv[i].release(envPF[5]);
  }



//  delay0.begin(tape_delay_bank, DELAY_MAX_LEN, 5000, 1, 2); //bank to use, size of bank, delay time in samples , rate reduction, interpolation time
  
  opmode=0;
}
