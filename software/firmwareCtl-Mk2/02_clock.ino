long bpm2Micros(int val){ //convert bpm to microseconds
return(60000000/val/24);
  }
void clck_strt(){
  mClock=-1 ;
  cmpClck(mClock);  
  clckOn=1;
  sndMidiClck(0);
}

void clck_stp(){
  mClock=-1 ;
  cmpClck(mClock);  
  clckOn=0;
  sndMidiClck(2);
  genSq_allNOff();
}


long bpm2Millis(int val){ //convert bpm to microseconds
return(60000/val/24);
  }

void chngBpm(float bpm){
  //intClockInt = bpm2Micros(bpm);
  intClockInt = bpm2Micros(bpm);
}

void updIntClock(){
  //long micro=micros();
  if (micros()-intClockTimer >= intClockInt && extClk==0){
    if(fbrdMode == 1 && opMode != strArp_opMode) mtOut = 1, sndVol(0);  
    else mtOut = 0, sndVol(vol); 
    if(clckOn){
      //intClockTimer+=intClockInt;
      //intClockTimer = micro;
      mClock++;
      sndMidiClck(1);
      cmpClck(mClock);        
    }
    updDisplay();
    intClockTimer+=intClockInt;
  }
}

void cmpClck(long mClock){ 
  pulse=mClock/24;
  bar=mClock/96;
  for (int i = 0;i<genSq_nInst;i++){
    if(schdPttnCh[i]>-1 && pulse != lastPulse){
      genSq_actPttn[i]=schdPttnCh[i];
      strArp_sync();
      genSq_sync(i);
      schdPttnCh[i]=-1;
    }
  }
  for (int i = 0;i<genSq_nInst;i++){
    if(schdSync[i]==1 && bar != lastBar){
      genSq_sync(i);
      strArp_sync();
      schdSync[i]=0;
    }
  }
  strArp_updClck(); 
  genSq_updClck();
  lastMClock=mClock;  
  lastPulse=pulse;  
  lastBar=bar;
  }
