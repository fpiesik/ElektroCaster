void updDisp(){
  if (millis()-frameTimer > frameInt){
    frmCnt++;
    mkColors();
    ledsTrgOff();
    if(frmCnt!=lastFrmCnt){
      switch (dispMode){
        case 4:
          if(nDispSrc==0)dispScale(1);
          if(nDispSrc==1)dispScale(0);
          //if(nDispSrc==1)dispTuner();
          //lightSlew();
          if(nDispSrc==1&&NotesOrTones==0)dispMidiTones();
          if(nDispSrc==1&&NotesOrTones==1)dispMidiNotes();
          lightSlew();
          if(arpSize==0)mkArp(arpMode);
          break;
        case 6:
          mkFrame();
          if(arpSize==0)mkArp(arpMode);
          break;
        case 7: 
          dispTuner();
          lightSlew();
          }
      
      if(ledsChngd()==1)FastLED.show();
      lastFrmCnt=frmCnt;
      gotOsc=0;
    }
    frameTimer=millis();
  }
}

void rstLghtHold(){
  for(int i=0;i<12;i++){
    lastMidiTones[i]=0;
    midiTones[i]=0;
  }
  for(int i=0;i<128;i++){
    lastMidiNotes[i]=0;
    midiNotes[i]=0;
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


//void dispScale(bool color){
//  if (scaleSel==0)dispTuner();
//  else{
//    float brightS=0.5; //brighness off the colored scale
//    float brightM=0.3; //brightness of the monocrome scale
//    for(int fret=0;fret<nLedFrets;fret++){
//      for(int s=0;s<nStrings;s++){ 
//        byte note=tuning[s]+fret;
//        byte idx=(note+12-rootNote)%12;
//        
//        if(scales[scaleSel][idx]==1){
//          if(color==1){
//            trgtC[s][fret][0]=tnClrs[note%12][0]*brightS;
//            trgtC[s][fret][1]=tnClrs[note%12][1]*brightS;
//            trgtC[s][fret][2]=tnClrs[note%12][2]*brightS;
//          }
//          if(color==0){
//            trgtC[s][fret][0]=brightM;
//            trgtC[s][fret][1]=brightM;
//            trgtC[s][fret][2]=brightM;
//          }
//        }
//      }  
//    }
//  }
//}

void dispScale(bool color){

  float brightS=0.12; //brighness off the colored scale
  float brightM=0.05; //brightness of the monocrome scale

  for(int i=0;i<nSclStp[scaleSel];i++){
    for(int s=0;s<nStrings;s++){ 
      for(int f=0;f<nLedFrets;f++){
        byte actPitch=(tuning[s]+f+12)%12;
        byte trnPitch=(tuning[s]+f+24-rootNote+scales[scaleSel][scaleStp])%12;
        if(scales[scaleSel][i]==trnPitch){
          if(color==1){
            trgtC[s][f][0]=tnClrs[actPitch][0]*brightS;
            trgtC[s][f][1]=tnClrs[actPitch][1]*brightS;
            trgtC[s][f][2]=tnClrs[actPitch][2]*brightS;
          }
          if(color==0){
            trgtC[s][f][0]=brightM;
            trgtC[s][f][1]=brightM;
            trgtC[s][f][2]=brightM;
          }
        }
        
      }       
    }
  }
}

void dispMidiTones(){
  bright=0.2;
  for(int n=0;n<128;n++){
    midiTones[n%12]=0;
  }
  for(int n=0;n<128;n++){
    if(midiNotes[n]>0)midiTones[n%12]=midiNotes[n];
  }
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      //int idx=(note-rootNote)%12;
      int idx=note%12;
      
      if(midiTones[note%12]>0){
        trgtC[i][fret][0]=tnClrs[idx][0]*bright;
        trgtC[i][fret][1]=tnClrs[idx][1]*bright;
        trgtC[i][fret][2]=tnClrs[idx][2]*bright;
      }
    }
  }
}

void dispMidiNotes(){
  bright=0.2;
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      //int idx=(note-rootNote)%12;
      int idx=note%12;
      
      if(midiNotes[note]>0){
        trgtC[i][fret][0]=tnClrs[idx][0]*bright;
        trgtC[i][fret][1]=tnClrs[idx][1]*bright;
        trgtC[i][fret][2]=tnClrs[idx][2]*bright;
      }
    }
  }
}

void dispTuner(){
  float inTnWin=0.08;
  for(int s=0;s<nStrings;s++){
    //byte fret=10*(strP[s]-tuning[s])+12.05;
    byte fret=strP[s]-tuning[s]+12.5;
    //int cIdx=int(strP[s]*10)%120;
    int cIdx=int(strP[s]+0.5)%12;
    float diver=abs(strP[s]-int(strP[s]+0.5))*3;
    float diverB=(strP[s]-int(strP[s]+0.5))*3;
    float diff=strP[s]-tuning[s];
    if(diver>1)diver=1;

//    trgtC[s][12][0]=tnClrs[tuning[s]%12][0];
//    trgtC[s][12][1]=tnClrs[tuning[s]%12][1];
//    trgtC[s][12][2]=tnClrs[tuning[s]%12][2];
    
    trgtC[s][12][0]=0.5;
    trgtC[s][12][1]=0.5;
    trgtC[s][12][2]=0.5;
    
//    trgtC[s][12][0]=tnClrs[tuning[s]%12][0];
//    trgtC[s][12][1]=tnClrs[tuning[s]%12][1];
//    trgtC[s][12][2]=tnClrs[tuning[s]%12][2];
    
    trgtC[s][0][0]=tnClrs[tuning[s]%12][0];
    trgtC[s][0][1]=tnClrs[tuning[s]%12][1];
    trgtC[s][0][2]=tnClrs[tuning[s]%12][2];
    
    
    if(strA[s]>0.03){
      if(diverB>inTnWin){
        trgtC[s][fret+1][0]=diver-inTnWin;
        trgtC[s][fret+1][1]=diver-inTnWin;
        trgtC[s][fret+1][2]=diver-inTnWin;
      }
      if(diverB<-inTnWin){
        trgtC[s][fret-1][0]=diver-inTnWin;
        trgtC[s][fret-1][1]=diver-inTnWin;
        trgtC[s][fret-1][2]=diver-inTnWin;
      }
//
//          if(strA[s]>0.03){
//      if(diff>0.05 && diff<0.5){
//        trgtC[s][13][0]=diver;
//        trgtC[s][13][1]=diver;
//        trgtC[s][13][2]=diver;
//      }
//      if(diff<-0.05 && diff>-0.5){
//        trgtC[s][11][0]=diver;
//        trgtC[s][11][1]=diver;
//        trgtC[s][11][2]=diver;
//      }
      
//      trgtC[s][fret][0]=tnClrs[cIdx][0]*(1-diver)+diver/2;
//      trgtC[s][fret][1]=tnClrs[cIdx][1]*(1-diver)+diver/2;
//      trgtC[s][fret][2]=tnClrs[cIdx][2]*(1-diver)+diver/2;    

      trgtC[s][fret][0]=tnClrs[cIdx][0];
      trgtC[s][fret][1]=tnClrs[cIdx][1];
      trgtC[s][fret][2]=tnClrs[cIdx][2]; 
    }
  }
}

void lightSlew(){
  float slew=0.3; //smaller=faster
  float thrsh=0.01;
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

      pixel(s,f,actC[s][f]);
    }
  }
}

bool ledsChngd(){
  bool chngd=0;
  for(int i=0; i<NUMPIXELS; i++){
    if(frtPix[i]!=lastFrtPix[i]){
      lastFrtPix[i]=frtPix[i];
      chngd=1;
    }    
  }
  return chngd;
}

void ledsClr(){
  for(int i=0;i<NUMPIXELS;i++){
    frtPix[i]=CRGB(0, 0, 0);
  }
}
void ledsTrgOff(){
  for(int s=0;s<nStrings;s++){  
    for(int f=0;f<nLedFrets;f++){
      trgtC[s][f][0]=0;
      trgtC[s][f][1]=0;
      trgtC[s][f][2]=0;
    }
  }
}
