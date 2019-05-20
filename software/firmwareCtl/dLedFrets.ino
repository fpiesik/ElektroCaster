void updDisp(){
  mkColors();
  if(frmCnt!=lastFrmCnt){
    if(dispMode==4){
      if(nDispSrc==1&&NotesOrTones==0)dispMidiTones();
      if(nDispSrc==1&&NotesOrTones==1)dispMidiNotes();
      if(nDispSrc==0)dispScale();
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
    FastLED.show();
    lastFrmCnt=frmCnt;
    gotOsc=0;
  }
}

void rstLghtHold(){
  for(int i=0;i<12;i++){
    lastMidiTones[i]=0;
  }
  for(int i=0;i<128;i++){
    lastMidiNotes[i]=0;
  }
}
void ton(byte ton, float color[3]){
  for(int i=0;i<nStrings;i++){  
    for(int f=0;f<nLedFrets;f++){
      byte fret=tuning[i]+f;
      if(fret%12==ton)pixel(i,f,color);
      }
    }
}

void pixel(byte s, byte f, float c[3]){
  frtPix[pixPos[s][f]]=CRGB(c[2]*blueC, c[0]*redC, c[1]*greenC);
}

void fret(byte f, float c[3]){
  for(int i=0;i<nStrings;i++){ 
    pixel(i,f,c); 
  }
}


void dispScale(){
  for(int fret=0;fret<nLedFrets;fret++){
    for(int s=0;s<nStrings;s++){ 
      byte note=tuning[s]+fret;
      byte idx=(note+12-rootNote)%12;
      if(frtGrids[frtGrid][idx]==1){
        trgtC[s][fret][0]=tnClrs[idx][0];
        trgtC[s][fret][1]=tnClrs[idx][1];
        trgtC[s][fret][2]=tnClrs[idx][2];
        }
      else{
        trgtC[s][fret][0]=0;
        trgtC[s][fret][1]=0;
        trgtC[s][fret][2]=0;
        }
    }  
  }
  lightSlew();
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
      int idx=(note-rootNote)%12;
      
      if(lghtHold==1 && lastMidiTones[note%12]>0){
        trgtC[i][fret][0]=0.2;
        trgtC[i][fret][1]=0.2;
        trgtC[i][fret][2]=0.2;
      }
      if(midiTones[note%12]>0){
        trgtC[i][fret][0]=tnClrs[idx][0];
        trgtC[i][fret][1]=tnClrs[idx][1];
        trgtC[i][fret][2]=tnClrs[idx][2];
      }
      if(midiTones[note%12]==0 && lastMidiTones[note%12]==0){
        trgtC[i][fret][0]=0;
        trgtC[i][fret][1]=0;
        trgtC[i][fret][2]=0;
      }
    }
  }
lightSlew();
}

void dispMidiNotes(){
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      int idx=(note-rootNote)%12;
      
      if(lghtHold==1 && lastMidiNotes[note]>0){
        trgtC[i][fret][0]=0.2;
        trgtC[i][fret][1]=0.2;
        trgtC[i][fret][2]=0.2;
      }
      if(midiNotes[note]>0){
        trgtC[i][fret][0]=tnClrs[idx][0];
        trgtC[i][fret][1]=tnClrs[idx][1];
        trgtC[i][fret][2]=tnClrs[idx][2];
      }
      if(midiNotes[note]==0 && lastMidiNotes[note]==0){
        trgtC[i][fret][0]=0;
        trgtC[i][fret][1]=0;
        trgtC[i][fret][2]=0;
      }
    }
  }
lightSlew();
}



void lightSlew(){
  static float c[nStrings][nLedFrets][3]; 
  float slew=0.25; //smaller=faster
  float thrsh=0.1;
  for(int s=0;s<nStrings;s++){  
    for(int f=0;f<nLedFrets;f++){
      if(trgtC[s][f][0]<actC[s][f][0])actC[s][f][0]=actC[s][f][0]*slew;
      if(actC[s][f][0]<=thrsh)actC[s][f][0]=0;
      if(trgtC[s][f][0]>=actC[s][f][0])actC[s][f][0]=trgtC[s][f][0];

      if(trgtC[s][f][1]<actC[s][f][1])actC[s][f][1]=actC[s][f][1]*slew;
      if(actC[s][f][1]<=thrsh)actC[s][f][1]=0;
      if(trgtC[s][f][1]>=actC[s][f][1])actC[s][f][1]=trgtC[s][f][1];

      if(trgtC[s][f][2]<actC[s][f][2])actC[s][f][2]=actC[s][f][2]*slew;
      if(actC[s][f][2]<=thrsh)actC[s][f][2]=0;
      if(trgtC[s][f][2]>=actC[s][f][2])actC[s][f][2]=trgtC[s][f][2];

      //if(c[s][f][0]!=actC[s][f][0] || c[s][f][1]!=actC[s][f][1] || c[s][f][2]!=actC[s][f][2]){
        pixel(s,f,actC[s][f]);
//        c[s][f][0]=actC[s][f][0];
//        c[s][f][1]=actC[s][f][1];
//        c[s][f][2]=actC[s][f][2];
      //}
    }
  }
}



void ledsClr(){
  for(int i=0;i<NUMPIXELS;i++){
    frtPix[i]=CRGB(0, 0, 0);
  }
}
