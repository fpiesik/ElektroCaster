


void readFretboard(int sensMode) {
  switch (sensMode){
    case 0:
      for (int f=0; f < nFrets; f++) {
        pinMode(frtPins[f], OUTPUT);
        digitalWrite(frtPins[f], LOW);
          for (int s=0; s < nStrings; s++) {
            frtState[f][s] = !digitalRead(strPins[s]);
          }
        pinMode(frtPins[f], INPUT);
      }
      break;
    case 1:
      for (int f=0; f < nFrets; f++) {
        pinMode(frtPins[f], OUTPUT);
        digitalWrite(frtPins[f], LOW);
        }
        for (int s=0; s < nStrings; s++) {
        frtState[1][s] = !digitalRead(strPins[s]);
      }
  }
  

  for (int s=0; s < nStrings; s++) {
    strPrs[s]=0;
    bool used=0;
    for (int f = nFrets-1; f >= 0; f--) {  
      if(frtState[f][s]==1 && used==0){
        if(f < nFrets)strPrs[s]=f+1;              
        //lastChng[s]=millis();
        used=1;
      }
      lastFrtState[f][s]=frtState[f][s];
    }
    if(strPrs[s]==lastStrPrs[s])strBnc[s]++;
    if(strPrs[s]!=lastStrPrs[s])strBnc[s]=0;
    lastStrPrs[s]=strPrs[s]; 
  }

//actions when string is pressed or released
  static long lastChng[nStrings];
  static long lastExStrPr[nStrings];
  for (int s=0; s < nStrings; s++) {
    if (strBnc[s]>=strBncs && millis()-lastChng[s]>fretMaskT && lastExStrPr[s]!=strPrs[s]){
      if (opStrMode == 1 && strArp_act==0){
        sndTrigEnv(s, strPrs[s]);
        if(strPrs[s] > 0) kick(s);
      }
      
      sndStrPrs(s,strPrs[s]);
      drmSq_plcSteps(s);
      //lastStrPrs[s]=strPrs[s];
      lastChng[s]=millis(); 
      lastExStrPr[s]=strPrs[s];
    }
    } 
  }

//void readStr(){
//
//  for (int f=0; f < nFrets; f++) {
//    pinMode(frtPins[f], OUTPUT);
//    digitalWrite(frtPins[f], LOW);
//
//    //pinMode(frtPins[f], INPUT);
//    }
//    for (int s=0; s < nStrings; s++) {
//    frtState[1][s] = !digitalRead(strPins[s]);
//      }
//
//  for (int s=0; s < nStrings; s++) {
//    strPrs[s]=0;
//    bool used=0;
//    for (int f = nFrets-1; f >= 0; f--) {  
//      if(frtState[f][s]==1 && used==0){
//        if(f < nFrets)strPrs[s]=f+1;              
//        //lastChng[s]=millis();
//        used=1;
//      }
//      lastFrtState[f][s]=frtState[f][s];
//    }
//    if(strPrs[s]==lastStrPrs[s])strBnc[s]++;
//    if(strPrs[s]!=lastStrPrs[s])strBnc[s]=0;
//    lastStrPrs[s]=strPrs[s]; 
//  }
//
////actions when string is pressed or released
//  static long lastChng[nStrings];
//  static long lastExStrPr[nStrings];
//  for (int s=0; s < nStrings; s++) {
//    if (strBnc[s]>=strBncs && millis()-lastChng[s]>fretMaskT && lastExStrPr[s]!=strPrs[s]){
//      if (strArp_act==0){
//        sndTrigEnv(s, strPrs[s]);
//        if(strPrs[s] > 0) kick(s);
//      }
//      
//      sndStrPrs(s,strPrs[s]);
//      drmSq_plcSteps(s);
//      //lastStrPrs[s]=strPrs[s];
//      lastChng[s]=millis(); 
//      lastExStrPr[s]=strPrs[s];
//    }
//    } 
//  }
  
void rcvHidD(byte idx, int val){
  hidDVal[idx]=val;
  if(hidDVal[idx]!=lastHidDVal[idx]){
    procHidDChng(idx, val);
    Serial.print("hidDVal: ");
    Serial.print(idx); 
    Serial.print(" "); 
    Serial.println(hidDVal[idx]);
  }
}

void procHidDChng(byte idx, bool val){
  switch(idx){
    case 0: 
      //display encoder button
        dispEncMode=!val;
        Serial.print("dispEncMode: ");
        Serial.println(dispEncMode);
      break;
    
    case 1:
      //button under display encoder
      shift=!val;
      break;
    
    case 2:
      //tripple switch left
      strArp_act = val;
      break;
    
    case 3:
      //tripple switch middle
      sndBowOn(val);
      break;
    
    case 4:  
      //tripple switch right
      kickOn=val;
      break;
    
    case 5:
      //right tripple button
      break;
    
    case 6:
      //middle tripple button
      if(val==1)schdSync=1;
      break;
    
    case 7:
      //left tripple transport button
      break;

    case 8:
      //pots switch 
      break;
    
    case 9:
      break;

    case 10:
      //pots Encoder button
      for(byte s=0;s<(nStrings);s++){
          drmSq_chStrBtn(s,val);
      }
      break;
      
    case 11:
      //switch next to display
      break;

    case 12: 
      frtb_sensMode=val;//switch next to next switch to display
      break;
  }
  
// string encoder buttons
  byte pO=13;
  for(byte i=pO;i<(nStrings+pO);i++){
    byte s=i-pO;
    if(hidDVal[pO+s]!=lastHidDVal[pO+s]){
        drmSq_chStrBtn(s,val);
    }
  }
  lastHidDVal[idx] = hidDVal[idx];
}

void rcvHidA(byte idx, int dVal){
  float val=dVal/200.0;
  hidAVal[idx]=val;
  if(hidAVal[idx]!=lastHidAVal[idx]){
    procHidAChng(idx, val);
    lastHidAVal[idx] = hidAVal[idx];
  }
}
void procHidAChng(byte idx, float val){
  switch(idx){
    case 0:
       sndLfo1(4,val);//if(hidAVal[0]!=lastHidAVal[0])sndEnv2(4,hidAVal[0]),lastHidAVal[0] = hidAVal[0];
      break;
    case 1:
      sndLfo1(3,val);
      break;
    
    case 2:
      sndLfo1(2,val);
      break;
    
    case 3: 
      sndLfo1(1,val);
      break;

    case 4:
      sndEnv2(3,val);
      break;

    case 5:
      sndEnv1(3,val);
      break;

    case 6:
      sndEnv2(1,val);
      break;
    case 7:
      sndEnv1(1,val);
      break;

    case 8:
      sndEnv2(6,val);
      break;

    case 9:
      sndEnv1(6,val);
      break;
    
    case 10:
      break;
    
    case 11:
      break;

    case 12:
      //sndFX(1,hidAVal[12]);
      break;

    case 13:
      vol=val;
      sndVol(vol);
      
      break;

    case 14:
      sndFilter(1,val);
      break;
    
    case 15:
      sndFilter(3,val);
      break;

    case 16:
      //sndFX(0,val);
      break;

    case 17:
      //sndFX(3,val)
      break;
    
    case 18:
      //sndFX(2,val);
      break;
  }
  lastHidAVal[idx] = hidAVal[idx];
}

void rcvHidR(byte idx, int val){
  hidRVal[idx]=val;
  if(hidRVal[idx]!=lastHidRVal[idx])procHidRChng(idx, val);
}
void procHidRChng(byte idx, byte val){  
  switch(idx){
    case 0:
      for(int s=0;s<nStrings;s++){
        strArp_tmDv[s]=strArp_tmDvs[val];
      }
      Serial.print("tmDv");
      Serial.println(strArp_tmDv[5]);
      break;
    
    case 1:
      fledMode=opMdMap[val];
      opStrMode=opMdMap[val];
      if(opStrMode==0||opStrMode==3)sndVol(0);
      if(opStrMode==1||opStrMode==2)sndVol(vol);
      break;
    
    case 2:
      opMode=opMdMap[val];
      //opStrMode=opMdMap[val];
      //if(opStrMode==0||opStrMode==3)sndVol(0);
      //if(opStrMode==1||opStrMode==2)sndVol(vol);
      Serial.print("opMode: ");
      Serial.println(val);
      break;
  }
  lastHidRVal[idx] = hidRVal[idx];
}

void rcvHidE(byte idx, long val){
  hidEVal4[idx]=hidEVal4[idx]+val;
  hidEVal[idx]=hidEVal4[idx]/4;
  if(hidEVal[idx]!=lastHidEVal[idx]){
    val = hidEVal[idx]-lastHidEVal[idx];
    procHidEChng(idx, val);
    Serial.print("hidEVal: ");
    Serial.print(idx); 
    Serial.print(" "); 
    Serial.println(hidEVal[idx]);
    lastHidEVal[idx] = hidEVal[idx];
  }
} 

void procHidEChng(byte idx, long val){ 
  switch(idx){
    case 0:
      switch(opMode){
        case 0:
          //???
          break;
        case 1:
          scls_chDispEnc(val);
          break;
        case 2:
          strArp_chDispEnc(val);
          break;
        case 3:
          drmSq_chDispEnc(val);
          break;
      }
      break;
    case 7:
      for(int s=0;s<=nStrings;s++){
        switch(opMode){
          case 0:
            tuning[s]=tuning[s]+val;
            break;
          case 1:
            break;
          case 2:
            strArp_chStrEnc(s, val);
            break;
          case 3:
            drmSq_chStrEnc(s, val);
            break;
        }
      }
      //bpm=abs(hidEVal[7]);
      //sndBpm(bpm);
      //intClockInt=bpm2Millis(bpm);
      break;
  }
  
  for(int i=1;i<=nStrings;i++){
    if(hidEVal[i]!=lastHidEVal[i]){
      int s=i-1;
      switch(opMode){
        case 0:
          tuning[s]=tuning[s]+val;
          break;
        case 1:
          break;
        case 2:
          strArp_chStrEnc(s, val);
          break;
        case 3:
          drmSq_chStrEnc(s, val);
          break;
      }
    }
  }

}

void  procChng(){
//  // send mode of operation to audio server
//  if(lastOpmode!=opmode){
//    sndMode(opmode);
//    if(opmode==2){
//      sndEnv1(4,0);
//      sndEnv2(4,0);
//    }
//    if(opmode==0){
//      sndEnv1(4,1);
//      sndEnv2(4,1);
//    }
//    lastOpmode=opmode;
//  }
//
//  if(dispMode!=lastDispMode){
//    ledsClr();
//    clrMTones();
//    if(lastDispMode==7)updEep();
//    lastDispMode=dispMode;
//  }
//
//  //actions when kickmode changes
//  if(kickMode!=lastKickMode){
//    sndKickMode(kickMode);
//    lastKickMode=kickMode;
//  }
//
//  if(runSeq!=lastRunSeq){
//    lastRunSeq=runSeq;
//  }
//
//  if(lghtHold!=lastLghtHold){
//    rstLghtHold();
//    lastLghtHold=lghtHold;
//  }
//
//  //actions when bow mode changes
//  if(bowMode!=lastBowMode){
//    sndBowMode(bowMode);
//    lastBowMode=bowMode;
//  }
}
