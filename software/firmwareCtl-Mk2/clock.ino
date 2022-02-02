long bpm2Micros(int val){ //convert bpm to microseconds
return(60000000/val/24);
  }
  
long bpm2Millis(int val){ //convert bpm to microseconds
return(60000/val/24);
  }

void chngBpm(float bpm){
  intClockInt = bpm2Micros(bpm);
}

void updIntClock(){
  if(extClk==0){
    if (micros()-intClockTimer >= intClockInt){
      intClockTimer=micros();
      mClock++;
      cmpClck(mClock);        
    }
  }
}

void cmpClck(long mClock){ 
  pulse=mClock/12;
  if(schdSync==1 && pulse != lastPulse){
    drmSq_sync();
    strArp_sync();
    schdSync=0;
  }
  drmSq_updClck();
  strArp_updClck(); 
  lastMClock=mClock;  
  lastPulse=pulse;  
  }



void midiClock() { 
  mClock++;
  cmpClck(mClock);
  }
void midiStart() {
  mClock=-1;
  extClk=1;
  cmpClck(mClock);
}
void midiStop() {
  mClock=-1 ;
  cmpClck(mClock);
}
