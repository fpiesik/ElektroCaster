long bpm2Micros(int val){
return(60000000/val/24);
}
long bpm2Millis(int val){
return(60000/val/24);
}

void midiClock() {
  clockMode=2; 
  mClock++;
  cmpClck(mClock);
}
void midiStart() {
clockMode=2;
mClock=-1;
}
void midiStop() {
mClock=-1 ;
allNOff();
}

void intClock(){
  if(clockMode==1){
    if(mClock==-1)sndClock(1);
    sndClock(0);
    mClock++;
    cmpClck(mClock);    
    intClockTimer=millis();    
  }
}

void cmpClck(byte clck){
 // static int lastSeqIdx;
  
  for(byte s=0;s<nStrings;s++){
    if(mClock==0)tdClock[s]=-1;
      switch(arpClkMode){
        case 0:
          mClockDev=mClock%tmDv[s];
          break;
        case 1:
          mClockDev=mClock%(tmDv[s]/arpRpt[arpSeq[seqIdx[s]]]);
          break;
        case 2:
          mClockDev=mClock%(tmDv[s]/arpSize);
          break;
        case 3:
          mClockDev=mClock%(tmDv[s]/arpRpt[s]);
          break;
    }     
    if(mClockDev==0)tdClock[s]++;       
  }
  
  if(runSeq==1)updSeq();
  //sndGetMidi();
  //sndGetHid();
  lastMClock=mClock;
  for(byte s=0;s<nStrings;s++){
    lastTdClock[s]=tdClock[s];
  }
}

void updSeq(){
  static byte lStr[nStrings];
  for(int s=0;s<nStrings; s++){
    if(lastTdClock[s]!=tdClock[s]){
      if(arpMode<=7)mkArp(arpMode);
      if(arpMode>7)mkSeq(arpMode);
      seqIdx[s]=tdClock[s]%seqNStp[s]+1;
      if(1){
        if(lStr[s]==1){
          strNote(s,0);
          lStr[s]=0;
        }
        if(arpMute[s]==0){
          if(strUsed[s]>0&&stepState[s][seqIdx[s]]>0){
            strNote(s,stepState[s][seqIdx[s]]);
            lStr[s]=1;
          }
          //if(stepState[s][seqIdx[s]]==1&&strUsed[s]>0&&kickMode==3)kickup(s);
          if(stepState[s][seqIdx[s]]==1&&strUsed[s]>0&&kickSeq==1)kickCue[s]=1;
        }
      }
    }
  }
}
