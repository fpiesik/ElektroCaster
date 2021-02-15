//void chDelay(byte para, float val){
//   if(para==0)delay0.length(val*DELAY_MAX_LEN);
//   if(para==1)mixer3.gain(1,val);
//   if(para==2)mixer4.gain(1,val);
//  }

void chFX(byte para, float val){
   if(para==0)outMix8.gain(1,val);
//   if(para==1)verb.roomsize(val);
//   if(para==2)verb.damping(val);
   if(para==3){
    for (int i=0; i<nStrings; i++) {
    preA[i]->gain(val);
  }
   }
}
