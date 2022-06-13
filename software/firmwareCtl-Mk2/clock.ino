long bpm2Micros(int val){ //convert bpm to microseconds
return(60000000/val/24);
  }
int clck_strt(){
  mClock=-1 ;
  cmpClck(mClock);  
  clckOn=1;
  sndMidiClck(0);
  intClockTimer=micros();
}

int clck_stp(){
//  mClock=-1 ;
//  cmpClck(mClock);  
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
  if (micros()-intClockTimer >= intClockInt && extClk==0){
    if(clckOn){
      intClockTimer+=intClockInt;
      mClock++;
      sndMidiClck(1);
      cmpClck(mClock);        
    }
    updDisplay();
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
