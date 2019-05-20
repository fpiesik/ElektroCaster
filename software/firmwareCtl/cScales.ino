void chRoot(){ 
//    rootNote=;
}

void chScale(){ 
//    actScale=
}


void clrMTones(){
  for(int i=0;i<12;i++){
    midiTones[i]=0;
  }
}




float pianoRollC[12][3]={
  {1,1,1},    //C
  {0,0,0},
  {1,1,1},  //D
  {0,0,0},
  {1,1,1},    //E
  {1,1,1},  //F
  {0,0,0},
  {1,1,1},    //G
  {0,0,0},    
  {1,1,1},  //A
  {0,0,0},
  {1,1,1}};   //H

  float frtGrids[nScales][12]={
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {1,0,1,0,1,1,0,1,0,1,0,1},
  {1,1,0,0,1,1,0,1,1,0,1,0}};
  
