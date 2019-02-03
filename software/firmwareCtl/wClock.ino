long bpm2Micros(int val){
return(60000000/val/24);
}

void rcvClock(long rClock){
  clockMode=2;
  mClock=rClock+1;
  cmpClck(mClock);
}

void intClock(){
  if(clockMode==1){
    if(mClock==-1)sndClock(1);
    sndClock(0);
    mClock++;
    cmpClck(mClock);   
  }
}

void cmpClck(byte clck){
  static int lastSeqIdx;
  
  for(byte s=0;s<nStrings;s++){
    if(mClock==0)tdClock[s]=-1;
      switch(arpClkMode){
        case 0:
          if(mClock%tmDv[s]==0)tdClock[s]++;
          break;
        case 1:
          if(mClock%(tmDv[s]/arpRpt[arpSeq[seqIdx[s]-1]])==0)tdClock[s]++;
          break;
        case 2:
          if(mClock%(tmDv[s]/arpSize)==0)tdClock[s]++;
          break;
        case 3:
          if(mClock%(tmDv[s]/arpRpt[s])==0)tdClock[s]++;
          break;
    }            
  }
  
  if(play==1)updSeq();
  sndGetHid();
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
      mkArp(arpMode);
      seqIdx[s]=tdClock[s]%seqNStp[s]+1;
      if(opmode==2){
        if(lStr[s]==1){
          strNote(s,0);
          lStr[s]=0;
        }
        if(arpMute[s]==0){
          if(rowUsed[s]>0&&stepState[s][seqIdx[s]]>0){
            strNote(s,stepState[s][seqIdx[s]]);
            lStr[s]=1;
          }
          if(stepState[s][seqIdx[s]]==1&&rowUsed[s]>0&&kickMode==3)kickup(s);
        }
      }
    }
  }
}
