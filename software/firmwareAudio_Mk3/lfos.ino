void chLfo1(byte para, float val,int str){
   static float lastVal[nStrings];
   static int octave;
   static float offst;
   static int note;
   
   switch (para){
    case 0:
      //lastVal[str]=val;
      note=val;
      lfo[str].frequency(midiFreq(note+octave*12+offst));
      break;
    case 1:
      lfo[str].amplitude(0.0001+val);
      lfo[str].offset(1.0-val);
      break;
    case 2:
    octave=val*9-5;
    lfo[str].frequency(midiFreq(note+octave*12+offst));
      break;
    case 3:
    offst=val*24-12;
    lfo[str].frequency(midiFreq(note+octave*12+offst));
      break;
    case 4:
      break;
   }
}
