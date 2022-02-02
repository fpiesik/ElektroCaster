void chEnvA(byte para, float val){
 //if (msg.isFloat(0)){
  
//envPA[para]=val;
  
  for (int i=0; i<nStrings; i++) {
    if(para==0)aEnv[i].delay(val);
    if(para==1)aEnv[i].attack(val);
    if(para==2)aEnv[i].hold(val);
    if(para==3)aEnv[i].decay(val);
    if(para==4)aEnv[i].sustain(val);
    if(para==5)aEnv[i].release(val);
    if(para==6){
      aBiasM[i].gain(0,val);
      aBiasM[i].gain(1,1.0-val);
      }
  }
}

void chEnvF(byte para, float val){

//envPF[para]=val;
  
  for (int i=0; i<nStrings; i++) {
    if(para==0)fEnv[i].delay(val);
    if(para==1)fEnv[i].attack(val);
    if(para==2)fEnv[i].hold(val);
    if(para==3)fEnv[i].decay(val);
    if(para==4)fEnv[i].sustain(val);
    if(para==5)fEnv[i].release(val);
    if(para==6){
      fBiasM[i].gain(0,val);
      fBiasM[i].gain(1,1.0-val);      
     }
  }
}
