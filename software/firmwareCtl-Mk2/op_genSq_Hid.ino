void genSq_chDispEnc(int val){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[genSq_actInst];
  switch(dispEncMode){
    case 0:
      switch(dispEncFnc[genSq_opMode]){
        case 0:
          if(genSq_strPrsFnc + val < 0)genSq_strPrsFnc=genSq_nStrPrsFnc;
          genSq_strPrsFnc=(genSq_strPrsFnc + val)%genSq_nStrPrsFnc;  
          break;
        case 1:
          if(genSq_strBtnFnc + val < 0)genSq_strBtnFnc=genSq_nStrBtnFnc;
          genSq_strBtnFnc=(genSq_strBtnFnc + val)%genSq_nStrBtnFnc;
          break;
        case 2:
          if(genSq_strEncFnc + val < 0)genSq_strEncFnc=genSq_nStrEncFnc;
          genSq_strEncFnc=(genSq_strEncFnc + val)%genSq_nStrEncFnc;
          break;
      }
      break;
    case 1:
      if(dispEncFnc[genSq_opMode] + val < 0)dispEncFnc[genSq_opMode] = nDispEncFnc[genSq_opMode];
      dispEncFnc[genSq_opMode]=(dispEncFnc[genSq_opMode] + val)%nDispEncFnc[genSq_opMode];
      break;
    case 2:
      if (shift==0){
        for(int s=0;s<nStrings;s++){
          if(genSq_stpEdtStrs[s]==1){
            if(genSq_strPrsFnc==genSq_strPrsFnc_sStp){
              if(genSq_sclQ[inst][s]==1){
                genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]=(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]+val)%scls_numSclStp[scls_sclSel];
                if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]<0)genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]=0;//scls_numSclStp[scls_sclSel]-1;
              }
              if(genSq_sclQ[inst][s]==0){
                genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]=(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]+val)%12;
                if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]<0)genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]=0;
              }
            }
            if(genSq_strPrsFnc!=genSq_strPrsFnc_sStp){
              genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]=(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]+val);
              if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]>genSq_maxStpV[genSq_strPrsFnc])genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]=genSq_maxStpV[genSq_strPrsFnc];
              if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]<0)genSq_stp[inst][pttn][s][genSq_stpEdtFrt][genSq_strPrsFnc]=0;
            }
            genSq_stpOnOff[inst][pttn][s][genSq_stpEdtFrt]=1;
          }
        }
      }
      if (shift==1){
        for(int stp=0;stp<genSq_maxSteps;stp++){
          for(int s=0;s<nStrings;s++){
            if(genSq_stpEdtStrs[s]==1){
              if(genSq_strPrsFnc==genSq_strPrsFnc_sStp){
                if(genSq_sclQ[inst][s]==1){
                  genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]=(genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]+val)%scls_numSclStp[scls_sclSel];
                  if(genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]<0)genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]=0;//scls_numSclStp[scls_sclSel]-1;
                }
                if(genSq_sclQ[inst][s]==0){
                  genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]=(genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]+val)%12;
                  if(genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]<0)genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]=0;
                }
              }
              if(genSq_strPrsFnc!=genSq_strPrsFnc_sStp){
                genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]=(genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]+val);
                if(genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]>genSq_maxStpV[genSq_strPrsFnc])genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]=genSq_maxStpV[genSq_strPrsFnc];
                if(genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]<0)genSq_stp[inst][pttn][s][stp][genSq_strPrsFnc]=0;
              }
              genSq_stpOnOff[inst][pttn][s][genSq_stpEdtFrt]=1;
            }
          }
        }
      }
      break; 
  }
}

void genSq_chStrEnc(int str, int val){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[genSq_actInst];
  if(strEncMode==0){
      if(genSq_strEncFnc_stps==genSq_strEncFnc){
        //for(int p=0;p<genSq_nPttn;p++){
          //pttn=p;
          int nSteps=genSq_chn[inst][pttn][str][genSq_strEncFnc_stps];
          nSteps = nSteps + val;
          if(nSteps < 1) nSteps=1;
          if(nSteps > genSeq_maxEncV[genSq_strEncFnc_stps]) nSteps=genSeq_maxEncV[genSq_strEncFnc_stps];
          genSq_chn[inst][pttn][str][genSq_strEncFnc_stps]=nSteps;
        //}
      }
      if(genSq_strEncFnc_tmDv==genSq_strEncFnc){
        //for(int p=0;p<genSq_nPttn;p++){
          //pttn=p;
          int tmDvSel = genSq_chn[inst][pttn][str][genSq_strEncFnc_tmDv];
          tmDvSel = tmDvSel + val;
          if(tmDvSel < 0) tmDvSel=0;
          if(tmDvSel >= genSeq_maxEncV[genSq_strEncFnc_tmDv])tmDvSel=genSeq_maxEncV[genSq_strEncFnc_tmDv]-1;
          genSq_tmDv[inst][pttn][str]=genSq_tmDvs[tmDvSel];
          genSq_chn[inst][pttn][str][genSq_strEncFnc_tmDv]=tmDvSel;
        //}
      }
      if(genSq_strEncFnc_chn==genSq_strEncFnc){
          int chn=genSq_chn[inst][pttn][str][genSq_strEncFnc_chn];
          chn = chn + val;
          if(chn < 0) chn=0;
          if(chn > genSeq_maxEncV[genSq_strEncFnc_chn]) chn=genSeq_maxEncV[genSq_strEncFnc_chn];
          genSq_chn[inst][pttn][str][genSq_strEncFnc_chn]=chn;
        //}
      }
      if(genSq_strEncFnc_sync==genSq_strEncFnc){
          int sync=genSq_chn[inst][pttn][str][genSq_strEncFnc_sync];
          sync = sync + val;
          if(sync < 0) sync=0;
          if(sync > genSeq_maxEncV[genSq_strEncFnc_sync]) sync=genSeq_maxEncV[genSq_strEncFnc_sync];
          genSq_chn[inst][pttn][str][genSq_strEncFnc_sync]=sync;
        //}
      }
  }
  if(strEncMode==1 && genSq_strEncChAStps==0 && nStrings-str-1<genSq_nStrPrsFnc){
    str=nStrings-str-1;
    genSq_strPrsFnc=str;
    for(int s=0;s<nStrings;s++){
      if(genSq_stpEdtStrs[s]==1){
        if(str==genSq_strPrsFnc_sStp){
          if(genSq_sclQ[inst][s]==1){
            genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]=(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]+val)%scls_numSclStp[scls_sclSel];
            if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]<0)genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]=0;//scls_numSclStp[scls_sclSel]-1;
          }  
          if(genSq_sclQ[inst][s]==0){
            genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]=(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]+val)%12;
            if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]<0)genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]=0;
          }    
        }
        if(str!=genSq_strPrsFnc_sStp){
          genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]=(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]+val);
          if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]>genSq_maxStpV[str])genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]=genSq_maxStpV[str];
          if(genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]<0)genSq_stp[inst][pttn][s][genSq_stpEdtFrt][str]=0;
        }
        genSq_stpOnOff[inst][pttn][s][genSq_stpEdtFrt]=1;
      }
    }
  }
  if(strEncMode==1 && genSq_strEncChAStps==1){
    //str=nStrings-str-1;
    for(int stp=0;stp<genSq_maxSteps;stp++){
      for(int s=0;s<nStrings;s++){
        if(genSq_stpEdtStrs[s]==1){
          if(str==genSq_strPrsFnc_sStp){
            if(genSq_sclQ[inst][s]==1){
              genSq_stp[inst][pttn][s][stp][str]=(genSq_stp[inst][pttn][s][stp][str]+val)%scls_numSclStp[scls_sclSel];
              if(genSq_stp[inst][pttn][s][stp][str]<0)genSq_stp[inst][pttn][s][stp][str]=0;//scls_numSclStp[scls_sclSel]-1;
            }
            if(genSq_sclQ[inst][s]==0){
              genSq_stp[inst][pttn][s][stp][str]=(genSq_stp[inst][pttn][s][stp][str]+val)%12;
              if(genSq_stp[inst][pttn][s][stp][str]<0)genSq_stp[inst][pttn][s][stp][str]=0;
            }    
          }
          if(str!=genSq_strPrsFnc_sStp){
            genSq_stp[inst][pttn][s][stp][str]=(genSq_stp[inst][pttn][s][stp][str]+val);
            if(genSq_stp[inst][pttn][s][stp][str]>genSq_maxStpV[str])genSq_stp[inst][pttn][s][stp][str]=genSq_maxStpV[str];
            if(genSq_stp[inst][pttn][s][stp][str]<0)genSq_stp[inst][pttn][s][stp][str]=0;
          }
          genSq_stpOnOff[inst][pttn][s][genSq_stpEdtFrt]=1;
        }
      }
    }
  }
}

void genSq_chStrBtn(byte s, bool val){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[genSq_actInst];
  
  switch(genSq_strBtnFnc){
    case genSq_strBtnFnc_mute:
      if(val==1)genSq_muteCh[inst][s]=!genSq_muteCh[inst][s];
      break;
    case genSq_strBtnFnc_rnd:
      if(val==1)genSq_rndCh(inst,pttn,s);
      break;
    case genSq_strBtnFnc_sclQ:
      if(val==1)genSq_sclQ[inst][s]=!genSq_sclQ[inst][s];
      break;
  }
}

void genSq_editSteps(byte s){
  int inst=genSq_actInst;
  int pttn=genSq_edtPttn[genSq_actInst];
  
  if(opMode>=genSq_opMode && opMode<genSq_opMode+genSq_nInst){   
    if(fbrdMode==1){
      if(strPrs[s]<=genSq_maxVisSteps){ 
        if(strPrs[s]==0){
          strEncMode=0;
          dispEncMode=0;
          genSq_stpEdtStrs[s]=0;
        }
        if(strPrs[s]>0){
          strEncMode=1;
          dispEncMode=2;
          genSq_stpEdtStr=s;
          genSq_stpEdtStrs[s]=1;
          genSq_stpEdtFrt=strPrs[s]-1;
          genSq_stpOnOff[inst][pttn][s][strPrs[s]-1]=(genSq_stpOnOff[inst][pttn][s][strPrs[s]-1]+1)%2;
          if(shift == 1)genSq_stpOnOff[inst][pttn][s][strPrs[s]-1]=0;
          //sndMidiCC(genSq_SelChnCC, 20,3);
          //sndMidiCC(genSq_SelChnCC, genSq_sndCh[inst][nStrings-s]*12-12,1);
        }
      }
    }
    
    if(strPrs[s]>genSq_pttnMOff-1 ){ 
      int instSel=genSq_nInst-s/2-1;
      int pttnSel=(strPrs[s]-(genSq_pttnMOff))+genSq_nPttn/2*((s+1)%2);
      if(dispEncMode == 1)genSq_cpPttn(genSq_actInst,genSq_actPttn[genSq_actInst],instSel,pttnSel);
      if(shift == 0)genSq_actPttn[instSel]=pttnSel;
      if(shift == 0 || shift == 1)genSq_edtPttn[instSel]=pttnSel; 
      genSq_actPttns[genSq_actPttnsIdx][instSel]=pttnSel;
      //genSq_sync(instSel); 
      genSq_actTmDv();
    }
  }
}

void genSq_actPttnsCh(){
  if(genSq_actPttnsIdx>=genSq_nActPttns)genSq_actPttnsIdx=0;
  int idx=genSq_actPttnsIdx;
  for(int i=0;i<genSq_nInst;i++){
    genSq_actPttn[i]=genSq_actPttns[idx][i];
    genSq_edtPttn[i]=genSq_actPttns[idx][i];
  }
}
