void genSq_updClck(){  
  for(int inst=0;inst<genSq_nInst;inst++){ 
    int pttn=genSq_actPttn[inst];
    static int tmDv[genSq_nInst][genSq_nPttn][nStrings];
    for(int s=0;s<nStrings;s++){  
      if(pulse != lastPulse && tmDv[inst][pttn][s] != genSq_tmDv[inst][pttn][s]){
        tmDv[inst][pttn][s]=genSq_tmDv[inst][pttn][s];
        genSq_nxtClkFil[inst][s]=genSq_tmDv[inst][pttn][s];
        //genSq_sync(inst); //todo: do i need this?
      }
      genSq_nxtClkFil[inst][s]++; //advance count up to next clock event
      if(mClock==-1)genSq_nxtClkFil[inst][s]=tmDv[inst][pttn][s]-1,genSq_clk[inst][s]=-1; //reset on mClock-reset  
      if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
        genSq_clk[inst][s]++;
        if(genSq_clk[inst][s]>=genSq_chn[inst][pttn][s][genSq_strEncFnc_stps])genSq_clk[inst][s]=0;
        //if(s==0 && genSq_muteCh[inst][s] == 0 && genSq_chn[inst][pttn][s][genSq_strEncFnc_chn]>0)genSq_mstStr(inst,pttn);
        genSq_mstStr(inst,pttn,s);
      }
    }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          genSq_sndCC(inst,pttn, s);
        }
      }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          genSq_sndStpOff(inst,pttn, s);
        }
      }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          if(genSq_muteCh[inst][s] == 0)genSq_sndStpOn(inst,pttn, s);
        }
      }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          genSq_nxtClkFil[inst][s]=0;
        }
      }
     for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          genSq_nxtClkFil[inst][s]=0;
        }
      }
    }
    

  
//            if(inst==0 && s==5){
//            Serial.print("inst: ");
//            Serial.print(inst);
//            Serial.print(" s: ");
//            Serial.print(s);
//            Serial.print(" clk: ");
//            Serial.println(genSq_clk[inst][s]);
//            }
  //Serial.print("genSq_clck: ");
  //Serial.println(genSq_clk[inst][0]);
  //Serial.println(genSq_nxtClkFil[inst][3]);
}

void genSq_sync(int inst){
  int pttn=genSq_actPttn[inst];
  
  for(int s = 0; s<nStrings; s++){
      genSq_nxtClkFil[inst][s]=genSq_tmDv[inst][pttn][s];
      genSq_clk[inst][s]=-1;
  }
}

void genSq_mstStr(int inst, int pttn, int s){
  int sync = genSq_chn[inst][pttn][s][genSq_strEncFnc_sync];
  if (sync > 0){
    int str = nStrings-sync;
    if (str != s){
      if(genSq_stpOnOff[inst][pttn][str][genSq_clk[inst][str]] > 0){
        genSq_nxtClkFil[inst][s]=genSq_tmDv[inst][pttn][s];
        if (s>str)genSq_clk[inst][s]=0;
        if (s<str)genSq_clk[inst][s]=1;
      }
    } 
  }
}



void genSq_rndCh(int inst,int pttn, byte s){
  byte rndBuf[nStrings][genSq_maxVisSteps];
  byte stpCnt=0;
  byte rndCnt=0;
  int nSteps=genSq_chn[inst][pttn][s][genSq_strEncFnc_stps];
  for(byte f=0;f<genSq_chn[inst][pttn][s][genSq_strEncFnc_stps];f++){
    rndBuf[s][f]=0;
  }

  for(byte f=0;f<nSteps;f++){
    if(genSq_stpOnOff[s][f]>0){
      rndBuf[s][random(nSteps)]=1;
      stpCnt++;
    }
  }

  for(byte f=0;f<nSteps;f++){
    if(rndBuf[s][f]>0){
      rndCnt++;
    }
  }
  while(rndCnt<stpCnt){
    rndCnt=0;
    rndBuf[s][random(nSteps)]=1;
    for(byte f=0;f<nSteps;f++){
      if(rndBuf[s][f]>0){
      rndCnt++;
      }
    }
  }
  
  for(int f=0;f<nSteps;f++){
    genSq_stpOnOff[inst][pttn][s][f]=rndBuf[s][f];
  }
}

void genSq_sndCC(int inst,int pttn, byte s){
  int chnl = genSq_chn[inst][pttn][s][genSq_strEncFnc_chn];
  if(genSq_stpOnOff[inst][pttn][s][genSq_clk[inst][s]] > 0){
    int stpV=genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_cc1];
    if(stpV>0){
      if(chnl==0)rootNote=stpV-1;
      if(chnl>0)sndMidiCC(genSq_ccMp[0],genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_cc1],chnl);
    }

    stpV=genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_cc2];
    if(stpV>0){
      if(chnl==0)scls_sclSel=stpV-1;
      if(chnl>0)sndMidiCC(genSq_ccMp[1],genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_cc2],chnl);
    }

    stpV=genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_cc3];
    if(stpV>0){
      if(chnl==0)scls_sclStp=stpV-1;
      if(chnl>0)sndMidiCC(genSq_ccMp[2],genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_cc3],chnl);
    }
  }
}

void genSq_sndStpOn(int inst,int pttn, byte s){
  //int chnl = genSq_sndCh[inst][nStrings-s-1];
  int chnl = genSq_chn[inst][pttn][s][genSq_strEncFnc_chn];
  if(genSq_stpOnOff[inst][pttn][s][genSq_clk[inst][s]] > 0 && chnl > 0){
    int stpV=genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_sStp];
    //int pitch=(scls_scls[scls_sclSel][(stpV+scls_sclStp)%scls_numSclStp[scls_sclSel]]+rootNote)%12;
    int actSStp=(stpV+scls_sclStp)%scls_numSclStp[scls_sclSel];
    int stpOff=scls_scls[scls_sclSel][scls_sclStp];
    int pitch;
    if(genSq_sclQ[inst][s]==1)pitch=(scls_scls[scls_sclSel][actSStp]+rootNote-stpOff+12)%12;
    if(genSq_sclQ[inst][s]==0)pitch=stpV%12;
    //int pitch=scls_scls[scls_sclSel][(genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_sStp]+scls_sclStp)%scls_numSclStp[scls_sclSel]];
    //int pitch=scls_scls[scls_sclSel][genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_sStp]];
    int oct=genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_oct];
    int note = oct*12+pitch;
    //int note = genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_sStp];
    int vel = genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_vel]*1.0/genSq_maxStpV[genSq_strPrsFnc_vel]*127.0; 
    sndMidiNote(note,vel, chnl);
    genSq_actNotes[inst][s][note]=vel;
    genSq_velState[inst][s] = vel;
    genSq_lastNote[inst][s] = note;
  }
}

void genSq_sndStpOff(int inst,int pttn, byte s){
    
  int chnl = genSq_chn[inst][pttn][s][genSq_strEncFnc_chn];
  if(genSq_stpOnOff[inst][pttn][s][genSq_clk[inst][s]] > 0){ 
    if(genSq_velState[inst][s] != 0){
      sndMidiNote(genSq_lastNote[inst][s],0, chnl);
      genSq_actNotes[inst][s][genSq_lastNote[inst][s]]=0;
    }
  }
  if(genSq_stpOnOff[inst][pttn][s][genSq_clk[inst][s]] == 0 && genSq_velState[inst][s] != 0){
    sndMidiNote(genSq_lastNote[inst][s],0, chnl);
    genSq_actNotes[inst][s][genSq_lastNote[inst][s]]=0;
    genSq_velState[inst][s] = 0;
  }
}

void genSq_cpPttn(int frmInst, int frmPttn, int toInst, int toPttn){
  for(int str = 0; str<nStrings; str++){
    for(int chFnc = 0; chFnc<genSq_nStrEncFnc; chFnc++){
      genSq_chn[toInst][toPttn][str][chFnc]=genSq_chn[frmInst][frmPttn][str][chFnc];
    }
    for(int stp = 0; stp<genSq_maxSteps; stp++){
      genSq_stpOnOff[toInst][toPttn][str][stp]=genSq_stpOnOff[frmInst][frmPttn][str][stp];
      for(int stpFnc = 0; stpFnc<genSq_nStrPrsFnc; stpFnc++){
        genSq_stp[toInst][toPttn][str][stp][stpFnc]=genSq_stp[frmInst][frmPttn][str][stp][stpFnc];
      }
    }
  }  
}
void genSq_actTmDv(){
  for(int inst = 0; inst<genSq_nInst; inst++){
    for(int pttn = 0; pttn<genSq_nPttn; pttn++){
      for(int str = 0; str<nStrings; str++){
        genSq_tmDv[inst][pttn][str]=genSq_tmDvs[genSq_chn[inst][pttn][str][genSq_strEncFnc_tmDv]];
      }
    }
  }
}

void genSq_allNOff(){
  for(int inst = 0; inst<genSq_nInst; inst++){
    for(int str = 0; str<nStrings; str++){
      for(int pttn = 0; pttn<genSq_nPttn; pttn++){
        genSq_sndStpOff(inst, pttn, str);
      }
    }
  }
}
