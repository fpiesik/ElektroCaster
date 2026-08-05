void applyAmpDecayForString(byte str, float val){
  aEnv[str].decay(val);
  if (manualEnvDecay[str] || val >= (sclEnvA[3] / 20.0 * 19)) aEnv[str].sustain(1);
  else aEnv[str].sustain(0);
}

void applyFilterDecayForString(byte str, float val){
  fEnv[str].decay(val);
  if (manualEnvDecay[str] || val >= (sclEnvF[3] / 20.0 * 19)) fEnv[str].sustain(1);
  else fEnv[str].sustain(0);
}

void chManualEnvDecay(byte str, byte state){
  if (str >= nStrings) return;
  manualEnvDecay[str] = state > 0;
  applyAmpDecayForString(str, envPA[3]);
  applyFilterDecayForString(str, envPF[3]);
}

void chEnvA(byte para, float val){
 //if (msg.isFloat(0)){
  
//envPA[para]=val;
  for (int i=0; i<nStrings; i++) {
    if(para==0)aEnv[i].delay(val);
    if(para==1)aEnv[i].attack(val);
    if(para==2)aEnv[i].hold(val);
    if(para==3)applyAmpDecayForString(i, val);
    if(para==4)aEnv[i].sustain(val);
    if(para==5)aEnv[i].release(val);
    if(para==6){
      aBiasM[i].gain(0,val);
      aBiasM[i].gain(1,1.0*envAAmnt[i]-val*envAAmnt[i]);
      }
  }
}

void chEnvF(byte para, float val){

//envPF[para]=val;
  
  for (int i=0; i<nStrings; i++) {
    if(para==0)fEnv[i].delay(val);
    if(para==1)fEnv[i].attack(val);
    if(para==2)fEnv[i].hold(val);
    if(para==3)applyFilterDecayForString(i, val);
    if(para==4)fEnv[i].sustain(val);
    if(para==5)fEnv[i].release(val);
    if(para==6){
      fBiasM[i].gain(0,val);
      fBiasM[i].gain(1,1.0*envFAmnt[i]-val*envFAmnt[i]);      
     }
  }
}
