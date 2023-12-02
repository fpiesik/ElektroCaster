void saveSong(int sng){
  SD.remove(genSq_SongNm[sng]);
  myFile = SD.open(genSq_SongNm[sng], FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    for(int inst = 0; inst<genSq_nInst; inst++){
      for(int str = 0; str<nStrings; str++){
        myFile.write(genSq_sclQ[inst][str]);
        for(int pttn = 0; pttn<genSq_nPttn; pttn++){
          for(int chFnc = 0; chFnc<genSq_nStrEncFnc; chFnc++){
            myFile.write(genSq_chn[inst][pttn][str][chFnc]);
          }
          for(int stp = 0; stp<genSq_maxSteps; stp++){
            myFile.write(genSq_stpOnOff[inst][pttn][str][stp]);
            for(int stpFnc = 0; stpFnc<genSq_nStrPrsFnc; stpFnc++){
              myFile.write(genSq_stp[inst][pttn][str][stp][stpFnc]);
            }
          }
        }
      }
    } 

    for(int p=0;p<genSq_nActPttns;p++){
      for(int i=0;i<genSq_nInst;i++){
        myFile.write(genSq_actPttns[p][i]);
      }
    }
    
    myFile.write(rootNote);
    myFile.write(scls_sclSel);
    myFile.write(scls_sclStp); 
    myFile.write(bpm);
    myFile.write(scls_fledSrc);
    for(int str = 0; str<nStrings; str++){
      myFile.write(tuning[str]);
    }
    for(int str = 0; str<nStrings; str++){
      myFile.write(strGain[str]);
    }
    
//    
//    Serial.print("Writing to test.txt...");
//    myFile.println("testing 1, 2, 3.");
//  // close the file:
    myFile.close();
    //Serial.print("done saving: ");
    //Serial.println(fileName);
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
  
}

void loadSong(int sng){ 
  myFile = SD.open(genSq_SongNm[sng]);
  if (myFile) {
    for(int inst = 0; inst<genSq_nInst; inst++){
      for(int str = 0; str<nStrings; str++){
        genSq_sclQ[inst][str]=myFile.read();
        for(int pttn = 0; pttn<genSq_nPttn; pttn++){
          for(int chFnc = 0; chFnc<genSq_nStrEncFnc; chFnc++){
            genSq_chn[inst][pttn][str][chFnc]=myFile.read();
            //delay(1);
          }
          for(int stp = 0; stp<genSq_maxSteps; stp++){
            genSq_stpOnOff[inst][pttn][str][stp]=myFile.read();
            //delay(1);
            for(int stpFnc = 0; stpFnc<genSq_nStrPrsFnc; stpFnc++){
              genSq_stp[inst][pttn][str][stp][stpFnc]=myFile.read();
              //delay(1);
            }
          }
        }
      }
    }
    for(int p=0;p<genSq_nActPttns;p++){
      for(int i=0;i<genSq_nInst;i++){
        genSq_actPttns[p][i]=myFile.read();
        if(genSq_actPttns[p][i]>genSq_nPttn)genSq_actPttns[p][i]=0;
      }
    }
    rootNote=myFile.read();
    scls_sclSel=myFile.read();
    scls_sclStp=myFile.read();
    bpm=myFile.read();
    chngBpm(bpm);
    scls_fledSrc=myFile.read();
    for(int str = 0; str<nStrings; str++){
      tuning[str] = myFile.read();
    }
    for(int s = 0; s<nStrings; s++){
      strGain[s] = myFile.read();
      sndStrGain(s,strGain[s]);
    }
    
    // close the file:
    myFile.close();
    genSq_actPttnsCh();
    
    for(int inst = 0; inst<genSq_nInst; inst++){
      for(int pttn = 0; pttn<genSq_nPttn; pttn++){
        for(int str = 0; str<nStrings; str++){
          genSq_tmDv[inst][pttn][str]=genSq_tmDvs[genSq_chn[inst][pttn][str][genSq_strEncFnc_tmDv]];
        }
      }
    }
//    Serial.print("done loading: ");
//    Serial.println(fileName);
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
}

void defaultSong(){
  for (int i=0; i < genSq_nInst; i++) {
    genSq_actPttn[i]=0;
    genSq_edtPttn[i]=0;
    for (int s=0; s < nStrings; s++) {
      genSq_sclQ[i][s]=0;
      for (int p=0; p < genSq_nPttn; p++) {
        genSq_chn[i][p][s][genSq_strEncFnc_tmDv]=10; 
        genSq_chn[i][p][s][genSq_strEncFnc_stps]=16;
        genSq_chn[i][p][s][genSq_strEncFnc_offSt]=0;
        genSq_chn[i][p][s][genSq_strEncFnc_sync]=0;
        genSq_chn[i][p][s][genSq_strEncFnc_chn]=i+1;        
        genSq_clk[i][s]=-1;
        for (int f=0; f < genSq_maxSteps; f++) {
          genSq_stpOnOff[i][p][s][f]=0;
          genSq_stp[i][p][s][f][genSq_strPrsFnc_sStp]=0;
          genSq_stp[i][p][s][f][genSq_strPrsFnc_oct]=3;
          genSq_stp[i][p][s][f][genSq_strPrsFnc_vel]=40; 
          genSq_stp[i][p][s][f][genSq_strPrsFnc_cc1]=0; 
          genSq_stp[i][p][s][f][genSq_strPrsFnc_cc2]=0; 
          genSq_stp[i][p][s][f][genSq_strPrsFnc_cc3]=0;     
          genSq_tmDv[i][p][s]=6;
        }
      }
    }
  }
  rootNote=0;
  scls_sclSel=3;
  scls_sclStp=0;
  bpm=90;
  chngBpm(bpm);
  scls_fledSrc=2;
  for(int str = 0; str<nStrings; str++){
    tuning[str] = defTuning[str];
  }
  for(int s = 0; s<nStrings; s++){
    strGain[s] = defStrGain[s];
    sndStrGain(s, strGain[s]);
  }
  genSq_actPttnsCh();
  for(int inst = 0; inst<genSq_nInst; inst++){
    for(int pttn = 0; pttn<genSq_nPttn; pttn++){
      for(int str = 0; str<nStrings; str++){
        genSq_tmDv[inst][pttn][str]=genSq_tmDvs[genSq_chn[inst][pttn][str][genSq_strEncFnc_tmDv]];
      }
    }
  }
}

void scanPttns(){
  static unsigned int inter=3000;
  if (millis()-scanPttnTimer > inter){
    scanPttnTimer+=inter;
    inter=500;
    static int pttn = 0;
    if(pttn<=genSq_nPttn){
      genSq_actPttn[0]=pttn;
      genSq_edtPttn[0]=pttn;
      genSq_actPttn[1]=pttn;
      genSq_edtPttn[1]=pttn;
      genSq_actPttn[2]=pttn;
      genSq_edtPttn[2]=pttn;
      pttn++;
    }
    if(pttn==genSq_nPttn+1){
      genSq_actPttn[0]=0;
      genSq_edtPttn[0]=0;
      genSq_actPttn[1]=0;
      genSq_edtPttn[1]=0;
      genSq_actPttn[2]=0;
      genSq_edtPttn[2]=0;
      pttn++;
    }
  }
}

void rstAllSngs(){
  defaultSong();
  for(int i=0;i<genSq_nSngs;i++){
    saveSong(i);
  }
}
