void drmSq_chDispEnc(int val){
  switch(dispEncMode){
    case 0:
      switch(dispEncFnc[opMode]){
        case 0:
          if(drmSq_strPrsFnc + val < 0)drmSq_strPrsFnc=drmSq_nStrPrsFnc;
          drmSq_strPrsFnc=(drmSq_strPrsFnc + val)%drmSq_nStrPrsFnc;  
          break;
        case 1:
          if(drmSq_strBtnFnc + val < 0)drmSq_strBtnFnc=drmSq_nStrBtnFnc;
          drmSq_strBtnFnc=(drmSq_strBtnFnc + val)%drmSq_nStrBtnFnc;
          break;
        case 2:
          if(drmSq_strEncFnc + val < 0)drmSq_strEncFnc=drmSq_nStrEncFnc;
          drmSq_strEncFnc=(drmSq_strEncFnc + val)%drmSq_nStrEncFnc;
          break;
      }
      break;
    case 1:
      if(dispEncFnc[opMode] + val < 0)dispEncFnc[opMode] = nDispEncFnc[opMode];
      dispEncFnc[opMode]=(dispEncFnc[opMode] + val)%nDispEncFnc[opMode];
      Serial.print("dispEncFnc[opMode] ");
      Serial.println(dispEncFnc[opMode]);
      break;
  }
}

void drmSq_chStrEnc(byte s, int val){
  switch(drmSq_strEncFnc){
    case 0:
      if(drmSq_nSteps[s] + val < 1) drmSq_nSteps[s]=drmSq_maxVisSteps+1;
      drmSq_nSteps[s] = (drmSq_nSteps[s] + val)%(drmSq_maxVisSteps+1);
      if(drmSq_nSteps[s]==0)drmSq_nSteps[s]=1;
      break;
    case 1:
      if(drmSq_tmDvSel[s] + val < 0) drmSq_tmDvSel[s]=drmSq_nTmDvs;
      drmSq_tmDvSel[s] = (drmSq_tmDvSel[s] + val)%drmSq_nTmDvs;
      drmSq_tmDv[s]=drmSq_tmDvs[drmSq_tmDvSel[s]];
      break;
  }
}

void drmSq_chStrBtn(byte s, bool val){
  switch(drmSq_strBtnFnc){
    case 0:
      if(val==1)drmSq_muteCh[s]=!drmSq_muteCh[s];
      break;
    case 1:
      if(val==1)drmSq_rndCh(s);
      break;
  }
}

void drmSq_updFleds(){
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<drmSq_maxVisSteps;f++){
      for(int ch=0;ch < 3; ch++){
        trgtC[s][f+1][ch]=drmSq_gridPix[s][f][ch]+drmSq_crsrPix[s][f][ch];
        if(drmSq_stp[s][f][0]>0)trgtC[s][f+1][ch]=drmSq_stpPix[s][f][ch]+drmSq_crsrPix[s][f][ch];
        if(drmSq_muteCh[s]==1)trgtC[s][f+1][ch]=trgtC[s][f+1][ch]*0.1;     
      }
    }
  }        
}

void drmSq_drwGrid(){
  float colorA[3]={0,0.2,1};
  float colorB[3]={0,0.02,0.1};
  
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<drmSq_maxVisSteps;f++){
      for(int c=0;c<3;c++){  
        drmSq_gridPix[s][f][c]=0;  
        if(f%4==0 && f<drmSq_nSteps[s]){
          drmSq_gridPix[s][f][c]=colorA[c];
        }
        if(f%4 != 0 && f<drmSq_nSteps[s]){
          drmSq_gridPix[s][f][c]=colorB[c];
        }
      }
    }
  }
}

void drmSq_rndCh(byte s){
  byte rndBuf[nStrings][drmSq_maxVisSteps];
  byte stpCnt=0;
  byte rndCnt=0;
  for(byte f=0;f<drmSq_nSteps[s];f++){
    rndBuf[s][f]=0;
  }

  for(byte f=0;f<drmSq_nSteps[s];f++){
    if(drmSq_stp[s][f][0]>0){
      rndBuf[s][random(drmSq_nSteps[s])]=1;
      stpCnt++;
    }
  }

  for(byte f=0;f<drmSq_nSteps[s];f++){
    if(rndBuf[s][f]>0){
      rndCnt++;
    }
  }
  while(rndCnt<stpCnt){
    rndCnt=0;
    rndBuf[s][random(drmSq_nSteps[s])]=1;
    for(byte f=0;f<drmSq_nSteps[s];f++){
      if(rndBuf[s][f]>0){
      rndCnt++;
      }
    }
  }
  
  for(byte f=0;f<drmSq_nSteps[s];f++){
    drmSq_stp[s][f][0]=rndBuf[s][f];
  }
  
}
void drmSq_plcSteps(byte s){
  if(opStrMode==drmSq_opMode){
  //static byte lastStrUsed[6];
    if(strPrs[s]>0){
      drmSq_stp[s][strPrs[s]-1][0]=(drmSq_stp[s][strPrs[s]-1][0]+1)%2;
      if(shift == 1)drmSq_stp[s][strPrs[s]-1][0]=0;
    }
    //lastStrUsed[s]=strUsed[s];
  }
}

void drmSq_updClck(){
  for(int s=0;s<nStrings;s++){ 
    static int tmDv[nStrings];
    if(pulse != lastPulse && tmDv[s] != drmSq_tmDv[s]){
      tmDv[s]=drmSq_tmDv[s];
      drmSq_nxtClkFil[s]=drmSq_tmDv[s];
      //drmSq_sync();
    }
    drmSq_nxtClkFil[s]++; //advance count up to next clock event
    if(mClock==-1)drmSq_nxtClkFil[s]=tmDv[s]-1,drmSq_clk[s]=0; //reset on mClock-reset  
    if (drmSq_nxtClkFil[s] >= tmDv[s]){
      drmSq_nxtClkFil[s]=0;
      if(s==0 && drmSq_muteCh[s] == 0)drmSq_mstStr();
      drmSq_drwGrid();
      if(drmSq_muteCh[s] == 0)drmSq_sndStp(s);
      drmSq_drwCursor(s);
      drmSq_clk[s]++;
      if(drmSq_clk[s]>=drmSq_nSteps[s])drmSq_clk[s]=0;
    }
    drmSq_drwStep(s);
  }
//  Serial.print("drmSq_clck: ");
//  Serial.println(drmSq_clk[3]);
}

void drmSq_drwCursor(byte s){
  float color[3] = {0,1,0};
  for(int f=0;f<drmSq_maxVisSteps;f++){
    for(int c=0;c<3;c++){    
      if(drmSq_clk[s]==f){
        drmSq_crsrPix[s][f][c]=color[c];
      }
      if(drmSq_clk[s]!=f){
        drmSq_crsrPix[s][f][c]=0;
      }
    }
  } 
}

void drmSq_drwStep(byte s){
  float color[3] = {1,0,1};
  for(int f=0;f<drmSq_maxVisSteps;f++){
    for(int c=0;c<3;c++){
      drmSq_stpPix[s][f][c]=0;    
      if(drmSq_stp[s][f][0]>0 && f<drmSq_nSteps[s]){
        drmSq_stpPix[s][f][c]=color[c];
      }
//      if(drmSq_stp[s][f][0]==0){
//        drmSq_stpPix[s][f][c]=0;
//      }
    }
  }
}

void drmSq_sync(){
  for(int s = 0; s<nStrings; s++){
      drmSq_nxtClkFil[s]=drmSq_tmDv[s];
      drmSq_clk[s]=0;
  }
}

void drmSq_mstStr(){
  if(drmSq_stp[0][drmSq_clk[0]][0] > 0){
    for(int s = 1; s<nStrings; s++){
      drmSq_nxtClkFil[s]=drmSq_tmDv[s];
      drmSq_clk[s]=0;
    }
  } 
}

void drmSq_sndStp(byte s){
  if(drmSq_stp[s][drmSq_clk[s]][0] > 0){
    byte note = 36 + s;
    if(drmSq_velState[s] != 0)sndMidiNote(note,0, 10);
    sndMidiNote(note,127, 10);
    drmSq_velState[s] = 127;
  }
  if(drmSq_stp[s][drmSq_clk[s]][0] == 0 && drmSq_velState[s] != 0){
    sndMidiNote(36+s,0, 10);
    drmSq_velState[s] = 0;
  }
}

void drmSq_updDisp(){
  //disp_Str(3, 8, "drmSq");
  disp_Color(1);
  switch (dispEncFnc[opMode]){
    case 0:
      if(dispEncMode==0)disp_RBox(0, 0, 128, 15, 3);
      if(dispEncMode==1)disp_RFrm(0, 0, 128, 15, 3);
      break;
    case 1:
      if(dispEncMode==0)disp_RBox(0, 15, 128, 15, 3);
      if(dispEncMode==1)disp_RFrm(0, 15, 128, 15, 3);
      break;
    case 2:
      if(dispEncMode==0)disp_RBox(0, 30, 128, 15, 3);
      if(dispEncMode==1)disp_RFrm(0, 30, 128, 15, 3);
      break;
  }
  disp_Color(dispEncMode==1||dispEncFnc[opMode]!=0);
  disp_Str(3, 9, "str:");
  disp_Str(45, 9, drmSq_strPrsNm[drmSq_strPrsFnc]);
  disp_Color(dispEncMode==1||dispEncFnc[opMode]!=1);
  disp_Str(3, 24, "btn");
  disp_Str(45, 24, drmSq_strBtnNm[drmSq_strBtnFnc]);
  disp_Color(dispEncMode==1||dispEncFnc[opMode]!=2);
  disp_Str(3, 39, "enc");
  disp_Str(45, 39, drmSq_strEncNm[drmSq_strEncFnc]);

  disp_Color(1);
  for(int s =0; s < nStrings;s++){
    if(drmSq_strEncFnc==0)disp_Int(108-s*21, 55, drmSq_nSteps[s]);
    if(drmSq_strEncFnc==1)disp_Str(108-s*21, 55, drmSq_tmDvNm[drmSq_tmDvSel[s]]);
  }
//  disp_Str(3, 55, drmSq_tmDvNm[drmSq_tmDvSel[5]]);
//  disp_Str(24, 55, drmSq_tmDvNm[drmSq_tmDvSel[4]]);
//  disp_Str(45, 55, drmSq_tmDvNm[drmSq_tmDvSel[3]]);
//  disp_Str(66, 55, drmSq_tmDvNm[drmSq_tmDvSel[2]]);
//  disp_Str(87, 55, drmSq_tmDvNm[drmSq_tmDvSel[1]]);
//  disp_Str(108, 55, drmSq_tmDvNm[drmSq_tmDvSel[0]]);
  
  disp_Color(1);
}
