void chRoot(){ 
  if(rowUsed[5]>0){
    rootNote=(rowUsed[5])%12;
  }
}

void chScale(){ 
  if(rowUsed[4]>0){
    actScale=rowUsed[4]-1;
  }
}
void dispScales(){

if(scale==0)ledsOff();
mkScale(actScale);
}

void clrMTones(){
  for(int i=0;i<12;i++){
    midiTones[i]=0;
  }
}

void mkScale(int scale){
float prime[3]={1,0.3,0.3};
float third[3]={1,1,0.3};
float fifth[3]={0.3,1,1};
float seventh[3]={1,0.3,1};
float neutral[3]={0.5,0.5,0.5};
float cOther[3]={0.05,0.05 ,0.05}; 
float cNot[3]={0,0,0}; 
float cTone[7][3]={{1,1,0},{0.1,0.1,0.1},{1,0.3,0.3},{0.1,0.1,0.1},{0.3,1,0.3},{0.1,0.1,0.1},{0.3,0.3,1}};

static byte lastScale;

if(scale==0)scale=lastScale;
if (scale==1){
ton((0+trans+rootNote)%12,prime);
ton((1+trans+rootNote)%12,cNot);
ton((2+trans+rootNote)%12,cOther);
ton((3+trans+rootNote)%12,cNot);
ton((4+trans+rootNote)%12,third);
ton((5+trans+rootNote)%12,cOther);
ton((6+trans+rootNote)%12,cNot);
ton((7+trans+rootNote)%12,fifth);
ton((8+trans+rootNote)%12,cNot);
ton((9+trans+rootNote)%12,cOther);
ton((10+trans+rootNote)%12,cNot);
ton((11+trans+rootNote)%12,seventh);  
}

if (scale==2){
ton((0+trans+rootNote)%12,prime);
ton((1+trans+rootNote)%12,cOther);
ton((2+trans+rootNote)%12,cNot);
ton((3+trans+rootNote)%12,cNot);
ton((4+trans+rootNote)%12,third);
ton((5+trans+rootNote)%12,cOther);
ton((6+trans+rootNote)%12,cNot);
ton((7+trans+rootNote)%12,fifth);
ton((8+trans+rootNote)%12,cOther);
ton((9+trans+rootNote)%12,cNot);
ton((10+trans+rootNote)%12,seventh);
ton((11+trans+rootNote)%12,cNot);  
}

if (scale==3){
ton((0+trans+rootNote)%12,neutral);
ton((1+trans+rootNote)%12,cNot);
ton((2+trans+rootNote)%12,neutral);
ton((3+trans+rootNote)%12,cNot);
ton((4+trans+rootNote)%12,neutral);
ton((5+trans+rootNote)%12,cNot);
ton((6+trans+rootNote)%12,neutral);
ton((7+trans+rootNote)%12,cNot);
ton((8+trans+rootNote)%12,neutral);
ton((9+trans+rootNote)%12,cNot);
ton((10+trans+rootNote)%12,neutral);
ton((11+trans+rootNote)%12,cNot); 
}

lastScale=scale;
//pixels.show();
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

float absToneC[12][3]={
  {1,0,0},    //C
  {1,0.25,0},
  {1,1,0},  //D
  {0.75,0.5,0},
  {0,1,0},    //E
  {0,1,1},  //F
  {0,1,0},
  {0,1,0.3},    //G
  {0,0.75,0.5},    
  {0,0.5,1},  //A
  {0,0,1},
  {0.4,0,1}};   //H
