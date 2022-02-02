void chFX(byte para, float val){
//   if(para==0)outMix8.gain(1,val);
//   if(para==1)verb.roomsize(val);
//   if(para==2)verb.damping(val);
   if(para==3){
    for (int i=0; i<nStrings; i++) {
    preA[i]->gain(val);
  }
   }
}
