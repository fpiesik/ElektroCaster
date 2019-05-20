void chLfo1(byte para, float val,int str){
   static float lastVal[nStrings];
   if(para==0){
    lastVal[str]=val;
    int note=strState[str]+tuning[str];
    midiFreq(note+val-sclLfo1[0]/2);
    lfos1[str]->frequency(midiF);
   }
    if(para==1){
    lfos1[str]->amplitude(0.0001+val);
    lfos1[str]->offset(1.0-val);
   }
   if(para==2){
    int note=strState[str]+tuning[str];
    midiFreq(note+lastVal[str]-sclLfo1[0]/2);
    lfos1[str]->frequency(midiF);
   }
}
