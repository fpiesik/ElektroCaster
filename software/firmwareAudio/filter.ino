void chFilter(byte para, float val){
  for(int i=0;i<nStrings;i++){
   if(para==0)filter[i].frequency(val);
   if(para==1)filter[i].resonance(val);
   if(para==3)filter[i].octaveControl(val);
  }
}
