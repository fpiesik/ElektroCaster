void updDisp(){
  if(frmCnt!=lastFrmCnt){
    if(opmode==1&&dispMode==2&&editMode==1)updateSteps();
    if(opmode==1&&dispMode==2&&editMode==2)updNStp();
    if(opmode==1&&dispMode==2&&editMode==3)updSeqSpd();
    if(dispMode==2)mkFrame();
    if(dispMode==1){
      if(shift==1)chRoot(),chScale();
      dispScales();  
    }
    if(dispMode==4){
      dispMidiTones();
      if(arpSize==0)mkArp(arpMode);
    }
    if(dispMode==5){
      dispMidiNotes();
      if(arpSize==0)mkArp(arpMode);
    }
    if(dispMode==6){
      mkFrame();
      if(arpSize==0)mkArp(arpMode);
    }
    pixels.show();
    lastFrmCnt=frmCnt;
    gotOsc=0;
  }
}


void ton(byte ton, float color[3]){
for(int i=0;i<nStrings;i++){  
for(int f=0;f<nLedFrets;f++){
  byte fret=tuning[i]+f;
if(fret%12==ton)pixels.setPixelColor(pixPos[i][f], pixels.Color(color[0]*redC,color[1]*greenC,color[2]*blueC));
if(fret%12==ton)pixels.setPixelColor(pixPos[i][f], pixels.Color(cComp(0,color[0]),cComp(1,color[1]),cComp(2,color[2])));
}}
}

void pixel(byte s, byte f, float c[3]){
  pixels.setPixelColor(pixPos[s][f], pixels.Color(cComp(0,c[0]),cComp(1,c[1]),cComp(2,c[2])));
}
void fret(byte f, float c[3]){
  for(int i=0;i<nStrings;i++){  
  pixels.setPixelColor(pixPos[i][f], pixels.Color(cComp(0,c[0]),cComp(1,c[1]),cComp(2,c[2])));
  }
}

void dispMidiTones(){
  for(int n=0;n<128;n++){
    midiTones[n%12]=0;
  }
  for(int n=0;n<128;n++){
    if(midiNotes[n]>0)midiTones[n%12]=midiNotes[n];
  }
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      //HSBtoRGB(n%12/12.0);
      HSBtoRGB((note-rootNote)%12/12.0);
      if(fret<nLedFrets&&midiTones[note%12]>0){
        trgtC[i][fret][0]=hsbRGB[0];
        trgtC[i][fret][1]=hsbRGB[1];
        trgtC[i][fret][2]=hsbRGB[2];
      }
      if(fret<nLedFrets&&midiTones[note%12]==0){
        float bright=0;
        byte idx=(note+12-rootNote)%12;
        if(idx==0)bright=0.35;
        if(idx!=0)bright=0.04;
        trgtC[i][fret][0]=pianoRollC[idx][0]*bright;
        trgtC[i][fret][1]=pianoRollC[idx][1]*bright;
        trgtC[i][fret][2]=pianoRollC[idx][2]*bright;
      }
    }
  }
lightSlew();
}

void dispMidiNotes(){
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      HSBtoRGB((note-rootNote)%12/12.0);
      if(midiNotes[note]>0){
        trgtC[i][fret][0]=hsbRGB[0];
        trgtC[i][fret][1]=hsbRGB[1];
        trgtC[i][fret][2]=hsbRGB[2];
      }
      if(fret<nLedFrets&&midiNotes[note]==0){
        float bright;
        int idx=note-rootNote;
        if(idx%12==0)bright=0.35;
        if(idx%12!=0)bright=0.04;
        trgtC[i][fret][0]=pianoRollC[idx%12][0]*bright;
        trgtC[i][fret][1]=pianoRollC[idx%12][1]*bright;
        trgtC[i][fret][2]=pianoRollC[idx%12][2]*bright;
      }
    }
  }
lightSlew();
}


void lightSlew(){
  float slew=0.6; //smaller=faster
  float thrsh=0.05;
  for(int s=0;s<nStrings;s++){  
    for(int f=0;f<nLedFrets;f++){
      float c[3];
      if(trgtC[s][f][0]<actC[s][f][0])actC[s][f][0]=actC[s][f][0]*slew;
      if(actC[s][f][0]<thrsh)actC[s][f][0]=0;
      if(trgtC[s][f][0]>=actC[s][f][0])actC[s][f][0]=trgtC[s][f][0];

      if(trgtC[s][f][1]<actC[s][f][1])actC[s][f][1]=actC[s][f][1]*slew;
      if(actC[s][f][1]<thrsh)actC[s][f][1]=0;
      if(trgtC[s][f][1]>=actC[s][f][1])actC[s][f][1]=trgtC[s][f][1];

      if(trgtC[s][f][2]<actC[s][f][2])actC[s][f][2]=actC[s][f][2]*slew;
      if(actC[s][f][2]<thrsh)actC[s][f][2]=0;
      if(trgtC[s][f][2]>=actC[s][f][2])actC[s][f][2]=trgtC[s][f][2];
      
      c[0]=actC[s][f][0];
      c[1]=actC[s][f][1];
      c[2]=actC[s][f][2];

      pixels.setPixelColor(pixPos[s][f], pixels.Color(cComp(0,c[0]),cComp(1,c[1]),cComp(2,c[2])));
    }
  }
}


byte cComp(byte color,float value){
  byte out;
  if(color==0)out=redC*value;
  if(color==1)out=greenC*value;
  if(color==2)out=blueC*value;
  if(out==0&&value>0)out=1;
  return out;
}





void ledsClr(){
  for(int i=0;i<NUMPIXELS;i++){  
  pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
}

void ledsOff(){
  for(int i=0;i<NUMPIXELS;i++){  
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 
  }
}
