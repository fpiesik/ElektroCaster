void procHid(){
  if (millis()-ctlTimer > ctlInt){ 
    if (hidDVal[12]!=lastHidDVal[12]){
      if(hidDVal[12]==0)nDispSrc=0;
      if(hidDVal[12]==1)nDispSrc=1;
      rstLghtHold();
      lastHidDVal[12] = hidDVal[12];
    }
  
      if (hidDVal[2]!=lastHidDVal[2]){
      if(hidDVal[2]==0)opmode=0;
      if(hidDVal[2]==1)opmode=2;
      lastHidDVal[2] = hidDVal[2];
    }
    
    if (hidDVal[11]!=lastHidDVal[11]){
      if(hidDVal[11]==0)dispMode=4;
      if(hidDVal[11]==1)dispMode=6;
      strEncFnc=1;
      lastHidDVal[11] = hidDVal[11];
    }
  
    if(hidAVal[9]!=lastHidAVal[9])sndEnv1(6,hidAVal[9]),lastHidAVal[9] = hidAVal[9];
    if(hidAVal[8]!=lastHidAVal[8])sndEnv2(6,hidAVal[8]),lastHidAVal[8] = hidAVal[8];
  
    if(hidAVal[7]!=lastHidAVal[7])sndEnv1(1,hidAVal[7]),lastHidAVal[7] = hidAVal[7];
    if(hidAVal[6]!=lastHidAVal[6])sndEnv2(1,hidAVal[6]),lastHidAVal[6] = hidAVal[6];
  
    if(hidAVal[5]!=lastHidAVal[5])sndEnv1(3,hidAVal[5]),lastHidAVal[5] = hidAVal[5];
    if(hidAVal[4]!=lastHidAVal[4])sndEnv2(3,hidAVal[4]),lastHidAVal[4] = hidAVal[4];
  
    if(hidAVal[3]!=lastHidAVal[3])sndLfo1(0,hidAVal[3]),lastHidAVal[3] = hidAVal[3];
    if(hidAVal[2]!=lastHidAVal[2])sndLfo1(1,hidAVal[2]),lastHidAVal[2] = hidAVal[2];
  //  if(hidAVal[1]!=lastHidAVal[1])sndEnv2(3,hidAVal[1]),lastHidAVal[1] = hidAVal[1];
  //  if(hidAVal[0]!=lastHidAVal[0])sndEnv2(4,hidAVal[0]),lastHidAVal[0] = hidAVal[0];
  
    //if(hidAVal[17]!=lastHidAVal[17])sndFilter(0,hidAVal[17]),lastHidAVal[17] = hidAVal[17];
    if(hidAVal[14]!=lastHidAVal[14])sndFilter(1,hidAVal[14]),lastHidAVal[14] = hidAVal[14];
    if(hidAVal[15]!=lastHidAVal[15])sndFilter(3,hidAVal[15]),lastHidAVal[15] = hidAVal[15];
    if(hidDVal[8]!=lastHidDVal[8])sndFilter(2,hidDVal[8]),lastHidDVal[8] = hidDVal[8];
  
    if(hidDVal[7]!=lastHidDVal[7]){
      if(hidDVal[7]==0){
        mClock=-1;
        runSeq=1;
      }
      lastHidDVal[7] = hidDVal[7];
    }
    if(hidDVal[6]!=lastHidDVal[6]){
      if(hidDVal[6]==0){
        sndClock(2);
        runSeq=0;
      }
      lastHidDVal[6] = hidDVal[6];
    }
  
    if(hidDVal[5]!=lastHidDVal[5]){
      if(hidDVal[5]==0){
        sndTrnsp(2,1);
        rstLghtHold();
      }
      
      if(hidDVal[5]==1)sndTrnsp(2,0);
      lastHidDVal[5] = hidDVal[5];
    }

     //Encoder switch 8
     if(hidDVal[10]!=lastHidDVal[10]){
      if(hidDVal[10]==0)dispMode=7;
      strEncFnc=0;
      lastHidDVal[10] = hidDVal[10];
    }
  
    for(byte s=0;s<nStrings;s++){
      byte pO=13;
      if(hidDVal[pO+s]!=lastHidDVal[pO+s]){
        if(hidDVal[pO+s]==0)arpMute[s]=!arpMute[s];
        lastHidDVal[pO+s] = hidDVal[pO+s];
  //      Serial.print("mute ");
  //      Serial.print(nStrings-s-1);
  //      Serial.print(" ");
  //      Serial.println(arpMute[nStrings-s-1]);
      }
    }
  
  
    if(hidAVal[16]!=lastHidAVal[16])sndFX(0,hidAVal[16]),lastHidAVal[16] = hidAVal[16];
    if(hidAVal[12]!=lastHidAVal[12])sndFX(1,hidAVal[12]),lastHidAVal[12] = hidAVal[12];
    if(hidAVal[18]!=lastHidAVal[18])sndFX(2,hidAVal[18]),lastHidAVal[18] = hidAVal[18];
    if(hidAVal[17]!=lastHidAVal[17])sndFX(3,hidAVal[17]),lastHidAVal[17] = hidAVal[17];
  
    if(hidAVal[13]!=lastHidAVal[13])sndVol(hidAVal[13]),lastHidAVal[13] = hidAVal[13];
  
    if(hidDVal[3]!=lastHidDVal[3])sndBowOn(hidDVal[3]),lastHidDVal[3] = hidDVal[3];
    if(hidDVal[4]!=lastHidDVal[4])kickSeq=hidDVal[4],lastHidDVal[4] = hidDVal[4];
  
    if(hidDVal[1]!=lastHidDVal[1])shift=!hidDVal[1],lastHidDVal[1] = hidDVal[1];
    
    if(hidDVal[0]!=lastHidDVal[0]){
        if(hidDVal[0]==1)dispEncFnc=(dispEncFnc+1)%nDispEncFnc;
        lastHidDVal[0] = hidDVal[0];  
    }
  
    if(hidRVal[0]!=lastHidRVal[0]){
      for(int s=0;s<nStrings;s++){
        tmDv[s]=tmDvs[hidRVal[0]];
      }
      lastHidRVal[0] = hidRVal[0];
    }
  
    if(hidRVal[1]!=lastHidRVal[1]){
      arpClkMode=hidRVal[1];
      lastHidRVal[1] = hidRVal[1];
    }
  
    if(hidRVal[2]!=lastHidRVal[2]){
      arpMode=hidRVal[2];
      lastHidRVal[2] = hidRVal[2];
    }
  
    if(hidEVal[0]!=lastHidEVal[0]&&dispEncFnc==0)rootNote=(rootNote + hidEVal[0]-lastHidEVal[0])%12,lastHidEVal[0] = hidEVal[0];
    if(hidEVal[0]!=lastHidEVal[0]&&dispEncFnc==1)scaleSel=(scaleSel + hidEVal[0]-lastHidEVal[0])%nScales,lastHidEVal[0] = hidEVal[0];
    if(hidEVal[0]!=lastHidEVal[0]&&dispEncFnc==2)scaleStp=(scaleStp + hidEVal[0]-lastHidEVal[0])%nSclStp[scaleSel],lastHidEVal[0] = hidEVal[0];
    
    if(hidEVal[7]!=lastHidEVal[7]){
      //cTimer.update(bpm2Micros((abs(hidEVal[7])))),lastHidEVal[7] = hidEVal[7];
      bpm=abs(hidEVal[7]);
      sndBpm(bpm);
      intClockInt=bpm2Millis(bpm);
      lastHidEVal[7] = hidEVal[7];
    }
  
    for(int i=1;i<=nStrings;i++){
      if(hidEVal[i]!=lastHidEVal[i]){
        int s=i-1;
        if(strEncFnc==0){
          tuning[s]=tuning[s]+hidEVal[i]-lastHidEVal[i],lastHidEVal[i] = hidEVal[i];
        }
        if(strEncFnc==1){
          if(arpMode<=7)arpRpt[s]=arpRpt[s]+hidEVal[i]-lastHidEVal[i],lastHidEVal[i] = hidEVal[i];
          if(arpMode>7)seqNDur[s]=seqNDur[s]+hidEVal[i]-lastHidEVal[i],lastHidEVal[i] = hidEVal[i];
        }
      }
      //if(hidEVal[i]!=lastHidEVal[i])arpRpt[(6-i)]=abs(hidEVal[i]),lastHidEVal[i] = hidEVal[i];
    }
    ctlTimer=millis();
  }
}

void  procChng(){
  // send mode of operation to audio server
  if(lastOpmode!=opmode){
    sndMode(opmode);
    if(opmode==2){
      sndEnv1(4,0);
      sndEnv2(4,0);
    }
    if(opmode==0){
      sndEnv1(4,1);
      sndEnv2(4,1);
    }
    lastOpmode=opmode;
  }

  if(dispMode!=lastDispMode){
    ledsClr();
    clrMTones();
    if(lastDispMode==7)updEep();
    lastDispMode=dispMode;
  }

  //actions when kickmode changes
  if(kickMode!=lastKickMode){
    sndKickMode(kickMode);
    lastKickMode=kickMode;
  }

  if(runSeq!=lastRunSeq){
    lastRunSeq=runSeq;
  }

  if(lghtHold!=lastLghtHold){
    rstLghtHold();
    lastLghtHold=lghtHold;
  }

  //actions when bow mode changes
  if(bowMode!=lastBowMode){
    sndBowMode(bowMode);
    lastBowMode=bowMode;
  }
}
