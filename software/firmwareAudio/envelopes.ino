void chEnvA(byte para, float val){
 //if (msg.isFloat(0)){
  
envPA[para]=val;
  
  for (int i=0; i<nStrings; i++) {
    if(para==0)aEnvs[i]->delay(val);
    if(para==1)aEnvs[i]->attack(val);
    if(para==2)aEnvs[i]->hold(val);
    if(para==3)aEnvs[i]->decay(val);
    if(para==4)aEnvs[i]->sustain(val);
    if(para==5)aEnvs[i]->release(val);
    if(para==6){
      aBiasM[i]->gain(0,val);
      aBiasM[i]->gain(1,1.0-val);
      }
  }
}

void chEnvF(byte para, float val){

envPF[para]=val;
  
  for (int i=0; i<nStrings; i++) {
    if(para==0)fEnvs[i]->delay(val);
    if(para==1)fEnvs[i]->attack(val);
    if(para==2)fEnvs[i]->hold(val);
    if(para==3)fEnvs[i]->decay(val);
    if(para==4)fEnvs[i]->sustain(val);
    if(para==5)fEnvs[i]->release(val);
    if(para==6){
      fBiasM[i]->gain(0,val);
//      delay(10);
      fBiasM[i]->gain(1,1.0-val);
//      delay(10);  
      //fBiasM[i]->gain(3,1);
//      aBiasM[i]->gain(0,val);
//      aBiasM[i]->gain(1,1.0-val);
       }
  }
}
