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
  pulse=mClock/12;
  if(schdSync==1 && pulse != lastPulse){
//    for(int i = 0;i<genSq_nInst;i++){
//      genSq_sync(i);
//    }
    genSq_sync(genSq_actInst);
    strArp_sync();
    schdSync=0;
  }
  strArp_updClck(); 
  genSq_updClck();
  lastMClock=mClock;  
  lastPulse=pulse;  
  }
