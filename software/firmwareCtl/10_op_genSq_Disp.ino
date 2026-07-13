void genSq_updFleds(){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[inst];
  
  genSq_drwGrid();
  for(int s=0;s<nStrings;s++){
    genSq_drwStep(s);
    genSq_drwCursor(s);
    int muteCh=genSq_muteCh[inst][s];
    
    for(int f=0;f<genSq_maxVisSteps;f++){
      bool stpOnOff=genSq_stpOnOff[inst][pttn][s][f];
      for(int ch=0;ch < 3; ch++){
        trgtC[s][f+1][ch]=genSq_gridPix[s][f][ch]+genSq_crsrPix[s][f][ch];
      }
      if(stpOnOff>0)for(int ch=0;ch < 3; ch++){
        trgtC[s][f+1][ch]=genSq_stpPix[s][f][ch]+genSq_crsrPix[s][f][ch];
      }
      if(muteCh==1)for(int ch=0;ch < 3; ch++){
        trgtC[s][f+1][ch]=trgtC[s][f+1][ch]*0.1;     
      }
    }
  }        
}

void genSq_drwGrid(){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[genSq_actInst];
  
  for(int s=0;s<nStrings;s++){
    int nSteps=genSq_chn[inst][pttn][s][genSq_strEncFnc_stps];
    int off=genSq_chn[inst][pttn][s][genSq_strEncFnc_offSt]; //offset
    for(int f=0;f<genSq_maxVisSteps;f++){
      for(int c=0;c<3;c++){  
        genSq_gridPix[s][f][c]=0;  
        if((f%4==0 && f>=off && f<nSteps+off) || f<off+nSteps-genSq_maxVisSteps){
          genSq_gridPix[s][f][c]=genSq_gridColor[inst][c]*5;
        }
        if((f%4 != 0 && f>=off && f<nSteps+off) || f<off+nSteps-genSq_maxVisSteps){
          genSq_gridPix[s][f][c]=genSq_gridColor[inst][c];
        }
      }
    }
  }
}

void genSq_drwCursor(byte s){
  int inst=genSq_actInst;
  float color[3] = {0.25,0.25,0.25};
  if(genSq_clk[inst][s]>=0){
    for(int c=0;c<3;c++){
      for(int f=0;f<genSq_maxVisSteps;f++){
        genSq_crsrPix[s][f][c]=0;
      }    
      genSq_crsrPix[s][genSq_clk[inst][s]][c]=color[c];
    }
  }
}

void genSq_drwStep(byte s){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[genSq_actInst];
  
  for(int f=0;f<genSq_maxVisSteps;f++){   
    int nSteps=genSq_chn[inst][pttn][s][genSq_strEncFnc_stps];
    int off=genSq_chn[inst][pttn][s][genSq_strEncFnc_offSt]; //offset
    int chnl = genSq_chn[inst][pttn][s][genSq_strEncFnc_chn];
    int stpV=genSq_stp[inst][pttn][s][f][genSq_strPrsFnc_sStp];
    float brght=genSq_stp[inst][pttn][s][f][genSq_strPrsFnc_vel]/float(genSq_maxStpV[genSq_strPrsFnc_vel]);
    //int pitch=(scls_scls[scls_sclSel][stpV]+rootNote)%12;
    int actSStp=(stpV+scls_sclStp)%scls_numSclStp[scls_sclSel];
    int stpOff=scls_scls[scls_sclSel][scls_sclStp];
    int pitch;
    if(genSq_sclQ[inst][s]==1)pitch=(scls_scls[scls_sclSel][actSStp]+rootNote-stpOff+12)%12;
    if(genSq_sclQ[inst][s]==0)pitch=stpV%12;
    //int pitch=scls_scls[scls_sclSel][(genSq_stp[inst][pttn][s][f][genSq_strPrsFnc_sStp]+scls_sclStp)%scls_numSclStp[scls_sclSel]];
    for(int c=0;c<3;c++){
      genSq_stpPix[s][f][c]=0;    
      if((genSq_stpOnOff[inst][pttn][s][f]>0 && f>=off && f<nSteps+off) || f<off+nSteps-genSq_maxVisSteps){
        if(chnl>0)genSq_stpPix[s][f][c]=tnClrs[pitch][c]*brght;
        if(chnl==0)genSq_stpPix[s][f][c]=brght/2;
      }
      if(genSq_stpOnOff[inst][pttn][s][f]==0){
        genSq_stpPix[s][f][c]=0;
      }
    }
  }
}

void genSq_updDisp(){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[genSq_actInst];
  
  //disp_Str(110, 9, "I");
  //disp_Int(120, 9, inst+1);

  //disp_Str(110, 24, "S");
  //disp_Int(120, 24, genSq_actSng);
  
  disp_Color(1);
  int boxW=68;
  switch (dispEncFnc[genSq_opMode]){
    case 0:
      if(dispEncMode==0)disp_RBox(0, 0, boxW, 15, 3);
      if(dispEncMode==1||dispEncMode==2)disp_RFrm(0, 0, boxW, 15, 3);
      break;
    case 1:
      if(dispEncMode==0)disp_RBox(0, 15, boxW, 15, 3);
      if(dispEncMode==1)disp_RFrm(0, 15, boxW, 15, 3);
      break;
    case 2:
      if(dispEncMode==0)disp_RBox(0, 30, boxW, 15, 3);
      if(dispEncMode==1)disp_RFrm(0, 30, boxW, 15, 3);
      break;
  }
  disp_Color(dispEncMode==1||dispEncFnc[genSq_opMode]!=0||dispEncMode==2);
  disp_Str(3, 9, "str:");
  disp_Str(35, 9, genSq_strPrsNm[genSq_strPrsFnc]);
  
  disp_Color(dispEncMode==1||dispEncFnc[genSq_opMode]!=1);
  disp_Str(3, 24, "btn");
  disp_Str(35, 24, genSq_strBtnNm[genSq_strBtnFnc]);
  
  disp_Color(dispEncMode==1||dispEncFnc[genSq_opMode]!=2);
  disp_Str(3, 39, "enc");
  disp_Str(35, 39, genSq_strEncNm[genSq_strEncFnc]);

  disp_Color(1);
  if(genSq_strEncFnc==genSq_strEncFnc_stps){
    for(int s =0; s < nStrings;s++)disp_Int(105-s*21, 55, genSq_chn[inst][pttn][s][genSq_strEncFnc_stps]);
  } 
  if(genSq_strEncFnc==genSq_strEncFnc_tmDv){
    for(int s =0; s < nStrings;s++)disp_Str(105-s*21, 55, genSq_tmDvNm[genSq_chn[inst][pttn][s][genSq_strEncFnc_tmDv]]);
  }
  if(genSq_strEncFnc==genSq_strEncFnc_offSt){
    for(int s =0; s < nStrings;s++)disp_Int(105-s*21, 55, genSq_chn[inst][pttn][s][genSq_strEncFnc_offSt]);
  }
    if(genSq_strEncFnc==genSq_strEncFnc_chn){
    for(int s =0; s < nStrings;s++)disp_Int(105-s*21, 55, genSq_chn[inst][pttn][s][genSq_strEncFnc_chn]);
  }
  if(genSq_strEncFnc==genSq_strEncFnc_sync){
    for(int s =0; s < nStrings;s++)disp_Int(105-s*21, 55, genSq_chn[inst][pttn][s][genSq_strEncFnc_sync]);
  }

  
  disp_Color(1);

  disp_Line(70, 0, 70, 44);
  int pitch;
  int stpOff=scls_scls[scls_sclSel][scls_sclStp]; 
  if(genSq_sclQ[inst][genSq_stpEdtStr]==1)pitch=(scls_scls[scls_sclSel][(genSq_stp[inst][pttn][genSq_stpEdtStr][genSq_stpEdtFrt][genSq_strPrsFnc_sStp]+scls_sclStp)%scls_numSclStp[scls_sclSel]]+rootNote-stpOff+12)%12;
  if(genSq_sclQ[inst][genSq_stpEdtStr]==0)pitch=genSq_stp[inst][pttn][genSq_stpEdtStr][genSq_stpEdtFrt][genSq_strPrsFnc_sStp]%12;

  disp_Str(73, 9, toneNm[pitch%12]);
  disp_Int(90, 9, genSq_stp[inst][pttn][genSq_stpEdtStr][genSq_stpEdtFrt][genSq_strPrsFnc_oct]+1);

  disp_Int(105, 9, genSq_stp[inst][pttn][genSq_stpEdtStr][genSq_stpEdtFrt][genSq_strPrsFnc_vel]);

  disp_Str(80, 39, "sclQ");
  disp_Int(113, 39, genSq_sclQ[inst][genSq_stpEdtStr]);
  
  disp_Int(73, 24, genSq_stp[inst][pttn][genSq_stpEdtStr][genSq_stpEdtFrt][genSq_strPrsFnc_cc1]);
  disp_Int(93, 24, genSq_stp[inst][pttn][genSq_stpEdtStr][genSq_stpEdtFrt][genSq_strPrsFnc_cc2]);
  disp_Int(113, 24, genSq_stp[inst][pttn][genSq_stpEdtStr][genSq_stpEdtFrt][genSq_strPrsFnc_cc3]);
  
}
