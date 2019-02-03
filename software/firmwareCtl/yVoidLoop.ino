void serialEvent1() { 
  if ( inbound.parseStream( &Serial1 ) ) {
    if ( inbound.fullMatch("c") ) {
      rcvClock(inbound.nextInt());
    }
    if ( inbound.fullMatch("n") ) {
      for(byte i=0;i<41;i++){
      byte vel=inbound.nextByte();
      rcvNote(i+35, vel);
      }
    }
    if ( inbound.fullMatch("cc") ) {
      byte cc=inbound.nextByte();
      byte val=inbound.nextByte();
      rcvCC(cc,val);
    }
    if ( inbound.fullMatch("f") ) {
      //sndGetHid();
      if(gotOsc>0&&frmCnt>lastFrmCnt)updDisp();
      gotOsc++;
      lastFrmCnt=frmCnt;
    }
  }
 }

void serialEvent3() { 
  if (inbound.parseStream( &Serial3 )) {
    if (inbound.fullMatch("a")) {
      byte idx = inbound.nextByte();
      int val = inbound.nextInt();
      rcvHidA(idx,val);
    }

    if (inbound.fullMatch("d")) {
      byte idx = inbound.nextByte();
      int val = inbound.nextByte();
      rcvHidD(idx,val);
    }
    if (inbound.fullMatch("r")) {
      byte idx = inbound.nextByte();
      int val = inbound.nextByte();
      rcvHidR(idx,val);
    }
    if (inbound.fullMatch("e")) {
      byte idx = inbound.nextByte();
      int val = inbound.nextLong();
      rcvHidE(idx,val);
    }
    if (inbound.fullMatch("f")) {
      if(gotOsc>0)updDisp();
      gotOsc++;
      sndGetMidi();
    }
  }
}


void loop() { 
    
  //control
  if (millis()-ctlTimer > ctlInt){ 

  if (hidDVal[12]!=lastHidDVal[12]){
    if(hidDVal[12]==0)opmode=0;
    if(hidDVal[12]==1)opmode=2;
    lastHidDVal[12] = hidDVal[12];
  }
  if (hidDVal[11]!=lastHidDVal[11]){
    if(hidDVal[11]==0)dispMode=4;
    if(hidDVal[11]==1)dispMode=6;
    lastHidDVal[11] = hidDVal[11];
  }

  if(hidAVal[9]!=lastHidAVal[9])sndLfo1(0,hidAVal[9]),lastHidAVal[9] = hidAVal[9];
  if(hidAVal[8]!=lastHidAVal[8])sndLfo1(1,hidAVal[8]),lastHidAVal[8] = hidAVal[8];

  if(hidAVal[7]!=lastHidAVal[7])sndEnv1(1,hidAVal[7]),lastHidAVal[7] = hidAVal[7];
  if(hidAVal[6]!=lastHidAVal[6])sndEnv1(2,hidAVal[6]),lastHidAVal[6] = hidAVal[6];
  if(hidAVal[5]!=lastHidAVal[5])sndEnv1(3,hidAVal[5]),lastHidAVal[5] = hidAVal[5];
  if(hidAVal[4]!=lastHidAVal[4])sndEnv1(4,hidAVal[4]),lastHidAVal[4] = hidAVal[4];

  if(hidAVal[3]!=lastHidAVal[3])sndEnv2(1,hidAVal[3]),lastHidAVal[3] = hidAVal[3];
  if(hidAVal[2]!=lastHidAVal[2])sndEnv2(2,hidAVal[2]),lastHidAVal[2] = hidAVal[2];
  if(hidAVal[1]!=lastHidAVal[1])sndEnv2(3,hidAVal[1]),lastHidAVal[1] = hidAVal[1];
  if(hidAVal[0]!=lastHidAVal[0])sndEnv2(4,hidAVal[0]),lastHidAVal[0] = hidAVal[0];

  if(hidAVal[17]!=lastHidAVal[17])sndFilter(0,hidAVal[17]),lastHidAVal[17] = hidAVal[17];
  if(hidAVal[14]!=lastHidAVal[14])sndFilter(1,hidAVal[14]),lastHidAVal[14] = hidAVal[14];
  if(hidAVal[15]!=lastHidAVal[15])sndFilter(3,hidAVal[15]),lastHidAVal[15] = hidAVal[15];
  if(hidDVal[8]!=lastHidDVal[8])sndFilter(2,hidDVal[8]),lastHidDVal[8] = hidDVal[8];

  if(hidDVal[7]!=lastHidDVal[7]){
    if(hidDVal[7]==0)mClock=-1,play=1;
    lastHidDVal[7] = hidDVal[7];
  }
  if(hidDVal[6]!=lastHidDVal[6]){
    if(hidDVal[6]==0)sndClock(2),play=0;;
    lastHidDVal[6] = hidDVal[6];
  }
  if(hidDVal[5]!=lastHidDVal[5]){
    if(hidDVal[5]==0)sndTrnsp(2,1);
    if(hidDVal[5]==1)sndTrnsp(2,0);
    lastHidDVal[5] = hidDVal[5];
  }

  for(byte s=0;s<nStrings;s++){
    byte pO=13;
    if(hidDVal[pO+s]!=lastHidDVal[pO+s]){
      if(hidDVal[pO+s]==0)arpMute[nStrings-s-1]=!arpMute[nStrings-s-1];
      lastHidDVal[pO+s] = hidDVal[pO+s];
//      Serial.print("mute ");
//      Serial.print(nStrings-s-1);
//      Serial.print(" ");
//      Serial.println(arpMute[nStrings-s-1]);
    }
  }

  if(hidAVal[16]!=lastHidAVal[16])sndDelay(0,hidAVal[16]),lastHidAVal[16] = hidAVal[16];
  if(hidAVal[12]!=lastHidAVal[12])sndDelay(1,hidAVal[12]),lastHidAVal[12] = hidAVal[12];
  if(hidAVal[18]!=lastHidAVal[18])sndDelay(2,hidAVal[18]),lastHidAVal[18] = hidAVal[18];

  if(hidAVal[13]!=lastHidAVal[13])sndVol(hidAVal[13]),lastHidAVal[13] = hidAVal[13];

  if(hidDVal[3]!=lastHidDVal[3])bowOn=hidDVal[3],sndBowOn(hidDVal[3]),lastHidDVal[3] = hidDVal[3];
  if(hidDVal[4]!=lastHidDVal[4])kickOn=hidDVal[4],lastHidDVal[4] = hidDVal[4];

  if(hidDVal[1]!=lastHidDVal[1])shift=!hidDVal[1],lastHidDVal[1] = hidDVal[1];

  if(hidRVal[0]!=lastHidRVal[0]){
    Serial.print("state ");
    Serial.println(hidRVal[0]);
    for(int s=0;s<nStrings;s++){
      tmDv[s]=tmDvs[hidRVal[0]];
    }
    lastHidRVal[0] = hidRVal[0];
  }

  if(hidRVal[1]!=lastHidRVal[1]){
    Serial.print("state ");
    Serial.println(hidRVal[1]);
    arpClkMode=hidRVal[1];
    lastHidRVal[1] = hidRVal[1];
  }

  if(hidRVal[2]!=lastHidRVal[2]){
    Serial.print("state ");
    Serial.println(hidRVal[2]);
    arpMode=hidRVal[2];
    lastHidRVal[2] = hidRVal[2];
  }

  if(hidEVal[0]!=lastHidEVal[0])rootNote=abs(hidEVal[0]),lastHidEVal[0] = hidEVal[0];
  if(hidEVal[7]!=lastHidEVal[7])Timer0.start(bpm2Micros((abs(hidEVal[7])))),lastHidEVal[7] = hidEVal[7];

  for(int i=1;i<=nStrings;i++){
    if(hidEVal[i]!=lastHidEVal[i])arpRpt[(6-i)]=abs(hidEVal[i]),lastHidEVal[i] = hidEVal[i];
  }
      
  ctlTimer=millis();
}
  
//frame
if (millis()-frameTimer > frameInt){
  frmCnt++;
  readFretboard();

  //calc order in which the strings were pressed
  for(int s=0;s<nStrings;s++){
    if(lastRowUsed[s]==0&&rowUsed[s]>0&&pOrder[pOrderIdx]!=s){
      pOrderIdx++;
      pOrder[pOrderIdx]=s; 
    }
  }

  for(int i=0;i<nStrings;i++){
    if (rowUsed[i]!=lastRowUsed[i])strFret(i, rowUsed[i]);
  }

  //change display mode
  for(int i=0;i<nStrings;i++){
    //if (ctlUsed[i]==1)dispMode=i+1; 
    if (ctlUsed[i]>0)editMode=ctlUsed[i];
  }

  // send mode of operation to audio server
  if(lastOpmode!=opmode){
    sndMode(opmode);
    lastOpmode=opmode;
  }

  //actions when dispmode changes
  if(dispMode!=lastDispMode){
    ledsOff();
    clrMTones();
    lastDispMode=dispMode;
  }

  //actions when kickmode changes
  if(kickMode!=lastKickMode){
    sndKickMode(kickMode);
    lastKickMode=kickMode;
  }

  //actions when bow mode changes
  if(bowMode!=lastBowMode){
    sndBowMode(bowMode);
    lastBowMode=bowMode;
  }

  frameTimer=millis();
  }
}
