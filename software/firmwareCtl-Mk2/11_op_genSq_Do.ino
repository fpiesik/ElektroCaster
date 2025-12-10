void genSq_updClck(){  
  for(int inst=0;inst<genSq_nInst;inst++){ 
    int pttn=genSq_actPttn[inst];
    static int tmDv[genSq_nInst][genSq_nPttn][nStrings];
    for(int s=0;s<nStrings;s++){  
      if(pulse != lastPulse && tmDv[inst][pttn][s] != genSq_tmDv[inst][pttn][s]){
        tmDv[inst][pttn][s]=genSq_tmDv[inst][pttn][s];
        genSq_nxtClkFil[inst][s]=genSq_tmDv[inst][pttn][s];
      }
      genSq_nxtClkFil[inst][s]++; //advance count up to next clock event
      if(mClock==-1)genSq_nxtClkFil[inst][s]=tmDv[inst][pttn][s]-1,genSq_clkraw[inst][s]=-1; //reset on mClock-reset  
      if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
        genSq_clkraw[inst][s]++;
        genSq_clk[inst][s]=((genSq_clkraw[inst][s]%genSq_chn[inst][pttn][s][genSq_strEncFnc_stps])+genSq_chn[inst][pttn][s][genSq_strEncFnc_offSt])%genSq_maxVisSteps; //apply the clock offset
        //genSq_mstStr(inst,pttn,s);
      }
    }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          genSq_sndCC(inst,pttn, s);
        }
      }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s] - 1){
          genSq_sndStpOff(inst,pttn, s);
          genSq_mstStr(inst,pttn,s);
          //genSq_mstStr(inst,pttn,s);
        }
      }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          if(genSq_muteCh[inst][s] == 0){
            genSq_sndStpOn(inst,pttn, s);
          }
        }
      }
      for(int s=0;s<nStrings;s++){
        if(genSq_nxtClkFil[inst][s] >= tmDv[inst][pttn][s]){
          genSq_nxtClkFil[inst][s]=0;
        }
      }
    }
}

void genSq_sync(int inst){
  int pttn=genSq_actPttn[inst];
  for(int s = 0; s<nStrings; s++){
      genSq_nxtClkFil[inst][s]=genSq_tmDv[inst][pttn][s];
      genSq_clkraw[inst][s]=-1;
  }
}

void genSq_mstStr(int inst, int pttn, int str){
  int npttn = genSq_chn[inst][pttn][str][genSq_strEncFnc_sync];
  int chnl = genSq_chn[inst][pttn][str][genSq_strEncFnc_chn];
  if (npttn > 0 && chnl == 0){
    //int str = nStrings-sync;
    // if (str != s){
    //   if(genSq_stpOnOff[inst][pttn][str][genSq_clk[inst][str]] > 0){
    //     genSq_nxtClkFil[inst][s]=genSq_tmDv[inst][pttn][s];
    //     if (s>str)genSq_clk[inst][s]=0;
    //     if (s<str)genSq_clk[inst][s]=1;
    //   }
    // } 
    if(genSq_stpOnOff[inst][pttn][str][genSq_clk[inst][str]] > 0){
      if(genSq_nxtClkFil[inst][str] = genSq_tmDv[inst][pttn][str]-1){
        //schdPttnCh[inst]=sync-1;
        //genSq_sndStpOff(inst,pttn, str);
        genSq_actPttn[inst]=npttn-1;
        genSq_sync(inst);
        //genSq_edtPttn[inst]=sync-1;
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
    float vel = genSq_stp[inst][pttn][s][genSq_clk[inst][s]][genSq_strPrsFnc_vel]*1.0/genSq_maxStpV[genSq_strPrsFnc_vel]; 
    int mvel=vel*127.0;
    
    if(inst!=0)sndMidiNote(note, mvel, chnl);
    
    //play sequence on the strings if pressed
    if(inst==0 && strSeq_act==1 && strPrs[s]>0 && fbrdMode == 0 && strHold[s]==0){          
      sndTrigEnv(s, vel);
      sndMidiNotePress(s,strPrs[s],chnl);
      kick(s);
    }
    //play only midi if string hold is activated
    if(inst==0 && strHold[s]==1 && fbrdMode == 0){          
      sndTrigEnv(s, 0);
      sndMidiNotePress(s,lastNZStrPrs[s],chnl);
      //sndMidiNote(note,mvel,mInst_chn);
      //kick(s);
    }
    //play if string pitch is not c 
    if(inst==0 && pitch>0 && strSeq_act==1 && strPrs[s]==0 && fbrdMode == 0){          
      sndTrigEnv(s, vel);
      sndMidiNotePress(s,strPrs[s],chnl);
      kick(s);
    }
    
    genSq_actNotes[inst][s][note]=mvel;
    genSq_velState[inst][s] = mvel;
    genSq_lastNote[inst][s] = note;
  }
}

void genSq_sndStpOff(int inst,int pttn, byte s){
    
  int chnl = genSq_chn[inst][pttn][s][genSq_strEncFnc_chn];
    if(genSq_velState[inst][s] != 0){
      if(inst!=0)sndMidiNote(genSq_lastNote[inst][s],0, chnl);
      if(inst==0 && strSeq_act==1)sndMidiNotePress(s,0,chnl);
      genSq_actNotes[inst][s][genSq_lastNote[inst][s]]=0;
    }
}

void genSq_sndStpOffNMW(int inst,int pttn, byte s){
    
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
