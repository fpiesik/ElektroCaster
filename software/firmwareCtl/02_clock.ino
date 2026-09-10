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
  if (micros()-intClockTimer >= intClockInt){
    if(fbrdMode == 1 && opMode != strArp_opMode) mtOut = 1, sndVol(0);  
    else mtOut = 0, sndVol(vol); 
    if(clckOn && extClk==0){
      //intClockTimer+=intClockInt;
      //intClockTimer = micro;
      mClock++;
      sndMidiClck(1);   
      cmpClck(mClock);     
    }
     
    intClockTimer+=intClockInt;
    updDisplay();
  }
}

void cmpClck(long mClock){ 
  pulse=mClock/24;
  bar=mClock/96;
  for (int i = 0;i<genSq_nInst;i++){
    syncPnt[i]=mClock/syncInt[i]; //sync point defined by the genSq_actTmDv of the first string (to do: should not be hardcoded to the first string)
  }
  
  if(schdStrArpModeSel > -1 && pulse != lastPulse && clckOn==1){
    strArp_modeSel = schdStrArpModeSel;
    updStrAutoMode();
    schdStrArpModeSel = -1;
  }
  if(schdStrArpModeSel > -1 && clckOn==0){
    strArp_modeSel = schdStrArpModeSel;
    updStrAutoMode();
    schdStrArpModeSel = -1;
  }
  
  if(schdStrArpPttnCh > -1 && pulse != lastPulse  && clckOn==1){
    strArp_nxtPttn = schdStrArpPttnCh;
    strArp_actPttn = strArp_nxtPttn;
    strArp_sync();
    schdStrArpPttnCh = -1;
  }
  if(schdStrArpPttnCh > -1 && clckOn==0){
    strArp_nxtPttn = schdStrArpPttnCh;
    strArp_actPttn = strArp_nxtPttn;
    strArp_sync();
    schdStrArpPttnCh = -1;
  }
  for (int i = 0;i<genSq_nInst;i++){
    if(schdPttnCh[i]>-1 && pulse != lastPulse && clckOn==1){
      genSq_nxtPttn[i]=schdPttnCh[i];
      genSq_actPttn[i]=genSq_nxtPttn[i];
      genSq_updTmDv();
      strArp_sync();
      genSq_sync(i);
      schdPttnCh[i]=-1;
    }
    if(schdPttnCh[i]>-1 && clckOn==0){
      genSq_nxtPttn[i]=schdPttnCh[i];
      genSq_actPttn[i]=genSq_nxtPttn[i];
      genSq_updTmDv();
      strArp_sync();
      genSq_sync(i);
      schdPttnCh[i]=-1;
    }
    if(schdSync[i]==1 && bar != lastBar){
      genSq_sync(i);
      strArp_sync();
      schdSync[i]=0;
    }
    //schedule pattern change by sync point
    if(schdSyncPnt[i]==1 && syncPnt[i] != lastSyncPnt[i]){
      genSq_actPttn[i] = genSq_nxtPttn[i];
      genSq_updTmDv();
      genSq_sync(i);
      strArp_sync();
      schdSyncPnt[i]=0;
      
    }
  }
  strArp_updClck(); 
  genSq_updClck();
  lastMClock=mClock;  
  lastPulse=pulse;  
  lastBar=bar;
  for (int i = 0;i<genSq_nInst;i++){
    lastSyncPnt[i]=syncPnt[i];
  }
 }
