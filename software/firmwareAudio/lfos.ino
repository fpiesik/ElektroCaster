void chLfo1(byte para, float val){
  for(int i=0;i<nStrings;i++){
   if(para==0)lfos1[i]->frequency(val);
   if(para==1)lfos1[i]->amplitude(val);
  }
}
