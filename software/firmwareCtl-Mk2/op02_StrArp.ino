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
    case 0:
      if(strArp_nRpt[s] + val < 1) strArp_nRpt[s]=1;
      strArp_nRpt[s] = strArp_nRpt[s] + val;
      break;
    case 1:
      if(strArp_tmDvSel[s] + val < 0) strArp_tmDvSel[s]=strArp_nTmDvs;
      strArp_tmDvSel[s] = (strArp_tmDvSel[s] + val)%strArp_nTmDvs;
      strArp_tmDv[s]=strArp_tmDvs[strArp_tmDvSel[s]];
      break;
  }
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
    if(strArp_strEncFnc==0)disp_Int(108-s*21, 55, strArp_nRpt[s]);
    if(strArp_strEncFnc==1)disp_Str(108-s*21, 55, strArp_tmDvNm[strArp_tmDvSel[s]]);
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

void strArp_ersStps(){
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      strArp_stp[s][f]=0;
    }
  }
}

void strArp_updClck(){
  if(fbrdMode==0){  
    strArp_mkArp();

    for(int s=0;s<nStrings;s++){
      static int tmDv[nStrings];
      if(pulse != lastPulse && tmDv[s] != strArp_tmDv[s]){
        tmDv[s]=strArp_tmDv[s];
        strArp_nxtClkFil[s]=strArp_tmDv[s];
        //drmSq_sync();
      }
      strArp_nxtClkFil[s]++;
      if (strArp_nxtClkFil[s] >= strArp_tmDv[s]){
        strArp_nxtClkFil[s]=0;
        
          
          //if(strPrs[s]==0)strArp_clk[s]=-1;
        if(strArp_stp[s][strArp_clk[s]]==1){
          sndTrigEnv(s, strPrs[s]);
          if(strPrs[s] > 0) kick(s);
        }
        strArp_clk[s]++;
        if(strArp_clk[s]>=strArp_nStps[s])strArp_clk[s]=0;
      }
    //if(strPrs[s] == 0)strArp_clk[s]=-1;
    }
    strArp_drwCursor();
    strArp_drwStep();
    //strArp_sndStp();
  //  Serial.print("drmSq_clck: ");
  //  Serial.println(drmSq_clk[3]);
  }
}

void strArp_sync(){
  for(int s = 0; s<nStrings; s++){
      strArp_nxtClkFil[s]=strArp_tmDv[s];
      strArp_clk[s]=0;
  }
}

void strArp_drwGrid(){
  float colorA[3]={0.5,0,0};
  float colorB[3]={0.05,0,0};
  
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      for(int c=0;c<3;c++){    
        if(f%4==0){
          strArp_gridPix[s][f][c]=colorA[c];
        }
        if(f%4 != 0){
          strArp_gridPix[s][f][c]=colorB[c];
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
      if(strArp_clk[s]>=0)strArp_crsrPix[s][strArp_clk[s]][c]=color[c];
    }
  }
}

void strArp_drwStep(){
  float color[3] = {1,0,1};
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<strArp_maxVisSteps;f++){
      for(int c=0;c<3;c++){    
        if(strArp_stp[s][f]==1){
          strArp_stpPix[s][f][c]=color[c];
        }
        if(strArp_stp[s][f]==0){
          strArp_stpPix[s][f][c]=0;
        }
      }
    }
  }
}

void strArp_mkArp(){
  byte flip=1;
  byte orderSrc=0;
  byte mirror=0;
  byte arpMode=2;
  byte arpSize=0;
  byte arpSeq[64];

  
//  if(arpMode==2||arpMode==3||arpMode==6||arpMode==7)orderSrc=1;
//  if(arpMode==1||arpMode==3||arpMode==5||arpMode==7)flip=1;
//  if(arpMode==4||arpMode==5||arpMode==6||arpMode==7)mirror=1;

  strArp_ersStps(); //clear the sequence
  arpSize=0; //reset arp size

  //calculate size of the sequence
  for(int s=0;s<nStrings;s++){
    if(strPrs[s]>0)arpSize=arpSize+strArp_nRpt[s];
  }
  
  int arpIdx=0;

  //-----make arp sequence by string order
  if(1){
    for(int s=0;s<nStrings;s++){
      if(strPrs[s]>0){
        for(int r=0;r<strArp_nRpt[s];r++){
          arpSeq[arpIdx]=s;
          arpIdx++;
        }  
      }
    }
  }

  //----flip sequence----
  if(flip==1){
    byte seqbuf[arpSize];
    for(int i=0;i<arpSize;i++){
      seqbuf[i]=arpSeq[arpSize-i-1];
    }
    for(int i=0;i<arpSize;i++){
      arpSeq[i]=seqbuf[i];
    }
  }

  if(mirror==1){
    for(int i=0;i<arpSize;i++){
      arpSeq[i+arpSize]=arpSeq[arpSize-i-1];
    }
    arpSize=arpSize*2;
  }
  
  //---copy arp sequence to sequencer----
  for(int i=0;i<arpSize;i++){
  strArp_stp[arpSeq[i]][i]=1;  
  }
  //---set seq length----
  for(int s=0;s<nStrings;s++){
  strArp_nStps[s]=arpSize;  
  }
}
