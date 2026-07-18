void strArp_chDispEnc(int val){
  switch(dispEncMode){
    case 0:
      switch(dispEncFnc[opMode]){
        case 0:
          if(strArp_strPrsFnc + val < 0)strArp_strPrsFnc=strArp_nStrPrsFnc;
          strArp_strPrsFnc=(strArp_strPrsFnc + val)%strArp_nStrPrsFnc;  
          break;
        case 1:
          if(strArp_strBtnFnc + val < 0)strArp_strBtnFnc=strArp_nStrBtnFnc;
          strArp_strBtnFnc=(strArp_strBtnFnc + val)%strArp_nStrBtnFnc;
          break;
        case 2:
          if(strArp_strEncFnc + val < 0)strArp_strEncFnc=strArp_nStrEncFnc;
          strArp_strEncFnc=(strArp_strEncFnc + val)%strArp_nStrEncFnc;
          break;
      }
      break;
    case 1:
      if(dispEncFnc[opMode] + val < 0)dispEncFnc[opMode] = nDispEncFnc[opMode];
      dispEncFnc[opMode]=(dispEncFnc[opMode] + val)%nDispEncFnc[opMode];
//      Serial.print("dispEncFnc[opMode] ");
//      Serial.println(dispEncFnc[opMode]);
      break;
  }
}

void strArp_chStrEnc(byte s, int val){
  switch(strArp_strEncFnc){
    case strArp_strEncFnc_stps:
      if(strArp_nRpt[s] + val < 1) strArp_nRpt[s]=1;
      strArp_nRpt[s] = strArp_nRpt[s] + val;
      break;
    case strArp_strEncFnc_tmDv:
      if(strArp_tmDvSel[s] + val < 0) strArp_tmDvSel[s]=strArp_nTmDvs;
      strArp_tmDvSel[s] = (strArp_tmDvSel[s] + val)%strArp_nTmDvs;
      strArp_tmDv[s]=strArp_tmDvs[strArp_tmDvSel[s]];
      break;
    case strArp_strEncFnc_chn:
      strArp_chn[s] = constrain(strArp_chn[s] + val, 0, 16);
      break;
    case strArp_strEncFnc_mode:
      strArp_mode[s] = strArp_mode[s] == strArp_modeSerial ? strArp_modeParallel : strArp_modeSerial;
      strArp_resetSerialCursor();
      break;
    case strArp_strEncFnc_order:
      strArp_order[s] = constrain(strArp_order[s] + val, 0, nStrings);
      strArp_resetSerialCursor();
      break;
  }
}

void strArp_chStrBtn(byte s, bool val){
  switch(strArp_strBtnFnc){
    case strArp_strBtnFnc_mute:
      if(val==1)strArp_muteCh[s]=!strArp_muteCh[s];
      break;
    case strArp_strBtnFnc_rnd:
      //if(val==1)strArp_rndCh(inst,pttn,s);
      break;
  }
}

void strArp_ersStps(){
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      strArp_stp[s][f]=0;
    }
  }
}

void strArp_updClck(){
  if(strArp_act == 1){  
    strArp_mkArp();

    strArp_updClckSerial();
    strArp_drwGrid();
    strArp_drwCursor();
    strArp_drwStep();
    //strArp_sndStp();
  //  Serial.print("drmSq_clck: ");
  //  Serial.println(drmSq_clk[3]);
  }
}


void strArp_resetSerialCursor(){
  strArp_serialStep=0;
  strArp_serialDisplayStep=-1;
  strArp_serialNxtClkFil=0;
  if(strArp_seqLen > 0)strArp_serialNxtClkFil=strArp_tmDv[strArp_seq[0]];
}

void strArp_notePressOrder(byte s, int press){
  if(press > 0){
    if(strArp_pressOrder[s] == 0){
      strArp_pressOrder[s] = strArp_pressOrderNext;
      strArp_pressOrderNext++;
    }
  }
  else{
    strArp_pressOrder[s] = 0;
    bool anyPressed=0;
    for(int i=0;i<nStrings;i++){
      if(strArp_pressOrder[i]>0)anyPressed=1;
    }
    if(!anyPressed)strArp_pressOrderNext=1;
  }
}

unsigned int strArp_orderGroupPressOrder(byte order){
  unsigned int groupPressOrder=0;
  for(int s=0;s<nStrings;s++){
    if(strPrs[s]>0 && strArp_order[s]==order && strArp_pressOrder[s]>0){
      if(groupPressOrder==0 || strArp_pressOrder[s]<groupPressOrder)groupPressOrder=strArp_pressOrder[s];
    }
  }
  return groupPressOrder;
}

void strArp_addStringToArp(byte arpSeq[], int *arpIdx, byte s){
  for(int r=0;r<strArp_nRpt[s] && *arpIdx<strArp_maxSteps;r++){
    arpSeq[*arpIdx]=s;
    (*arpIdx)++;
  }
}

void strArp_addPressedZeroOrderStrings(byte arpSeq[], int *arpIdx, bool zeroOrderAdded[], unsigned int beforePressOrder, bool addRemaining){
  for(unsigned int pressed=1;pressed<strArp_pressOrderNext;pressed++){
    for(int s=0;s<nStrings;s++){
      if(strPrs[s]>0 && strArp_order[s]==0 && strArp_pressOrder[s]==pressed && !zeroOrderAdded[s]){
        if(addRemaining || pressed<beforePressOrder){
          strArp_addStringToArp(arpSeq, arpIdx, s);
          zeroOrderAdded[s]=1;
        }
      }
    }
  }
}

bool strArp_isSameOrderGroup(byte a, byte b){
  return a != b && strArp_order[a] > 0 && strArp_order[a] == strArp_order[b];
}

bool strArp_shouldPlayString(byte s){
  return strPrs[s] > 0 && strArp_muteCh[s] == 0 && mtOut == 0 && strPrs[s] <= nFrets-genSq_nPttn/2-1;
}

void strArp_silenceInactiveSerialNotes(){
  if(frtb_sensMode!=0)return;

  for(int i = 0; i<nStrings; i++){
    if(!strArp_shouldPlayString(i)){
      int chnl = strArp_chn[i];
      sndMidiNotePress(i,0,chnl);
    }
  }
}


void strArp_updClckSerial(){
  strArp_silenceInactiveSerialNotes();

  if(strArp_seqLen == 0){
    for(int s = 0; s<nStrings; s++){
      strArp_clk[s]=-1;
    }
    strArp_resetSerialCursor();
    return;
  }

  if(strArp_serialStep >= strArp_seqLen)strArp_serialStep=0;
  if(strArp_serialDisplayStep >= strArp_seqLen)strArp_serialDisplayStep=-1;
  byte s = strArp_seq[strArp_serialStep];
  // Time the next trigger from the currently playing step, not the upcoming one.
  byte durationString = s;
  if(strArp_serialDisplayStep >= 0)durationString=strArp_seq[strArp_serialDisplayStep];
  strArp_serialNxtClkFil++;
  
  //if(strArp_serialNxtClkFil == strArp_tmDv[durationString]-1){
  //  if(strPrs[s] > 0 && strArp_muteCh[s]==0 && mtOut==0 && strPrs[s]<=nFrets-genSq_nPttn/2-1){
  //    if(frtb_sensMode==0)sndMidiNotePress(s,0,chnl);
  //  }
  //}
  
  if(strArp_serialNxtClkFil >= strArp_tmDv[durationString]){
    strArp_serialNxtClkFil=0;
    if(frtb_sensMode==0){
      for(int i = 0; i<nStrings; i++){
        if(i != s && !strArp_isSameOrderGroup(s, i)){
          int offChnl = strArp_chn[i];
          sndMidiNotePress(i,0,offChnl);
        }
      }
    }
    for(int i = 0; i<nStrings; i++){
      if(i == s || strArp_isSameOrderGroup(s, i)){
        int playChnl = strArp_chn[i];
        if(frtb_sensMode==0 && strPrs[i]==0)sndMidiNotePress(i,0,playChnl);
        if(strArp_shouldPlayString(i)){
          if(frtb_sensMode==0)sndMidiNotePress(i,strPrs[i],playChnl);
          sndTrigEnv(i, 1);
          kick(i);
        }
      }
    }
    strArp_serialDisplayStep=strArp_serialStep;
    strArp_serialStep++;
    if(strArp_serialStep>=strArp_seqLen)strArp_serialStep=0;
  }

  for(int i = 0; i<nStrings; i++){
    strArp_clk[i]=-1;
  }
  if(strArp_serialDisplayStep >= 0){
    byte cursorString = strArp_seq[strArp_serialDisplayStep];
    for(int i = 0; i<nStrings; i++){
      if(i == cursorString || strArp_isSameOrderGroup(cursorString, i)){
        strArp_clk[i]=strArp_serialDisplayStep;
      }
    }
  }
}

void strArp_sync(){
  for(int s = 0; s<nStrings; s++){
      strArp_nxtClkFil[s]=strArp_tmDv[s];
      strArp_clk[s]=0;
  }
  strArp_resetSerialCursor();
}

void strArp_updDisp(){
  //disp_Str(3, 8, "strArp");
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
  disp_Str(45, 9, strArp_strPrsNm[strArp_strPrsFnc]);
  disp_Color(dispEncMode==1||dispEncFnc[opMode]!=1);
  disp_Str(3, 24, "btn");
  disp_Str(45, 24, strArp_strBtnNm[strArp_strBtnFnc]);
  disp_Color(dispEncMode==1||dispEncFnc[opMode]!=2);
  disp_Str(3, 39, "enc");
  disp_Str(45, 39, strArp_strEncNm[strArp_strEncFnc]);

  disp_Color(1);
  for(int s =0; s < nStrings;s++){
    if(strArp_strEncFnc==strArp_strEncFnc_stps)disp_Int(108-s*21, 55, strArp_nRpt[s]);
    if(strArp_strEncFnc==strArp_strEncFnc_tmDv)disp_Str(108-s*21, 55, strArp_tmDvNm[strArp_tmDvSel[s]]);
    if(strArp_strEncFnc==strArp_strEncFnc_chn)disp_Int(108-s*21, 55, strArp_chn[s]);
    if(strArp_strEncFnc==strArp_strEncFnc_mode)disp_Str(108-s*21, 55, strArp_modeNm[strArp_mode[s]]);
    if(strArp_strEncFnc==strArp_strEncFnc_order)disp_Int(108-s*21, 55, strArp_order[s]);
  }

  disp_Color(1);
}

void strArp_updFleds(){
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      for(int ch=0;ch < 3; ch++){
        trgtC[s][f+1][ch]=strArp_gridPix[s][f][ch]+strArp_crsrPix[s][f][ch];
        if(strArp_stp[s][f]>0)trgtC[s][f+1][ch]=strArp_stpPix[s][f][ch]+strArp_crsrPix[s][f][ch];      
      }
    }
  }        
}

void strArp_drwGrid(){
  float colorA[3]={0.5,0,0};
  float colorB[3]={0.05,0,0};
  float colorMA[3]={0.1,0,0};
  float colorMB[3]={0.02,0,0};

  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      for(int c=0;c<3;c++){    
        if(f%4==0){
          if(strArp_muteCh[s]==0)strArp_gridPix[s][f][c]=colorA[c];
          else strArp_gridPix[s][f][c]=colorMA[c];
        }
        if(f%4 != 0){
          if(strArp_muteCh[s]==0)strArp_gridPix[s][f][c]=colorB[c];
          else strArp_gridPix[s][f][c]=colorMB[c];
        }
      }
    }
  }
}


void strArp_drwCursor(){
  float color[3] = {0,1,0};
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      for(int c=0;c<3;c++){    
          strArp_crsrPix[s][f][c]=0;
        }
      }
    }
   for(int s=0;s<nStrings;s++){
    for(int c=0;c<3;c++){    
      if(strArp_clk[s]>=0 && strArp_clk[s]<strArp_maxVisSteps)strArp_crsrPix[s][strArp_clk[s]][c]=color[c];
    }
  }
}

void strArp_drwStep(){
  float color[3] = {1,0,1};
  float colorM[3] = {0.2,0.2,0.2};
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      for(int c=0;c<3;c++){    
        if(strArp_stp[s][f]==1){
          if(strArp_muteCh[s]==0)strArp_stpPix[s][f][c]=color[c];
          if(strArp_muteCh[s]==1)strArp_stpPix[s][f][c]=colorM[c];
        }
        if(strArp_stp[s][f]==0){
          strArp_stpPix[s][f][c]=0;
        }
      }
    }
  }
}

void strArp_mkArp(){
  byte back=0;
  byte mirror=0;
  byte revMirror=0;
  byte arpSize=0;
  byte arpSeq[64];

  if(strArp_strPrsFnc==strArp_strPrsFnc_back)back=1;
  if(strArp_strPrsFnc==strArp_strPrsFnc_mirror)mirror=1;
  if(strArp_strPrsFnc==strArp_strPrsFnc_revMirror)revMirror=1;

  strArp_ersStps(); //clear the sequence
  arpSize=0; //reset arp size

  int arpIdx=0;
  bool zeroOrderAdded[nStrings];
  for(int s=0;s<nStrings;s++)zeroOrderAdded[s]=0;

  //-----make arp sequence by configurable string order
  if(1){
    for(int order=1;order<=nStrings;order++){
      unsigned int groupPressOrder=strArp_orderGroupPressOrder(order);
      if(groupPressOrder>0)strArp_addPressedZeroOrderStrings(arpSeq, &arpIdx, zeroOrderAdded, groupPressOrder, 0);
      for(int s=0;s<nStrings;s++){
        if(strPrs[s]>0 && strArp_order[s]==order){
          strArp_addStringToArp(arpSeq, &arpIdx, s);
          break;
        }
      }
    }
    strArp_addPressedZeroOrderStrings(arpSeq, &arpIdx, zeroOrderAdded, 0, 1);
  }

  arpSize=arpIdx;

  //----back sequence----
  if(back==1){
    byte seqbuf[arpSize];
    for(int i=0;i<arpSize;i++){
      seqbuf[i]=arpSeq[arpSize-i-1];
    }
    for(int i=0;i<arpSize;i++){
      arpSeq[i]=seqbuf[i];
    }
  }

  if(mirror==1 || revMirror==1){
    int baseSize=arpSize;
    byte seqbuf[baseSize];
    for(int i=0;i<baseSize;i++){
      seqbuf[i]=arpSeq[i];
    }
    if(revMirror==1){
      for(int i=0;i<baseSize;i++){
        arpSeq[i]=seqbuf[baseSize-i-1];
      }
    }
    for(int i=0;i<baseSize && arpSize<strArp_maxSteps;i++){
      arpSeq[baseSize+i]=seqbuf[baseSize-i-1];
      if(revMirror==1)arpSeq[baseSize+i]=seqbuf[i];
      arpSize++;
    }
  }

  strArp_seqLen=arpSize;
  for(int i=0;i<arpSize;i++){
    strArp_seq[i]=arpSeq[i];
  }
  if(strArp_serialStep>=strArp_seqLen)strArp_serialStep=0;

  //---copy serial arp sequence to sequencer----
  for(int i=0;i<arpSize;i++){
    byte stepString = arpSeq[i];
    strArp_stp[stepString][i]=1;
    for(int s=0;s<nStrings;s++){
      if(strArp_isSameOrderGroup(stepString, s))strArp_stp[s][i]=1;
    }
  }
  //---set seq length----
  for(int s=0;s<nStrings;s++){
    strArp_nStps[s]=arpSize;
  }
}
