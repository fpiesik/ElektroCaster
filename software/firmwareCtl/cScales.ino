void clrMTones(){
  for(int i=0;i<12;i++){
    midiTones[i]=0;
  }
}

  
//  bool scales[nScales][12]={
//  {0,0,0,0,0,0,0,0,0,0,0,0}, //Tuner
//  {1,0,1,0,1,1,0,1,0,1,0,1}, //ionian
//  {1,0,0,1,0,1,0,1,0,0,1,0}, //minor pentatonic
//  {1,0,1,0,1,0,0,1,0,1,0,0}, //major pentatonic
//  {1,1,0,0,1,1,0,1,1,0,1,0}, // mixo b9 b13
//  {1,0,1,0,1,0,1,0,1,0,1,0}, // whole tone
//  {1,0,1,1,0,1,1,0,1,1,0,1}, // whole tone half tone
//  {1,1,0,1,1,0,1,1,0,1,1,0}, // half tone whole tone
//  {1,1,0,1,1,0,1,0,1,0,1,0} // altered
//  }; 

  int scales[nScales][12]={
  {0}, //Root
  {0,2,4,5,7,9,11}, //ionian
  {0,3,5,7,10}, //pentatonic
  {0,1,4,5,7,8,10}, // mixo b9 b13
  {0,2,4,6,8,10}, // whole tone
  {0,2,3,5,6,8,9,11}, // whole tone half tone
  {0,1,3,4,6,8,10} // altered
  }; 

int nSclStp[nScales]= {1, 7, 5, 7, 6, 8, 7}; 
