
int scls_scls[nScales][12]={
{}, //off
{0}, //Root
{0,3,5,7,10}, //pentatonic
{0,2,4,5,7,9,11}, //dur
{0,2,3,5,7,8,10}, // moll
{0,2,3,5,7,8,11}, // hrm moll
{0,2,3,5,7,9,11}, // mel moll
{0,1,3,4,6,8,10}, // altered
{0,2,4,6,8,10}, // whole tone
{0,2,3,5,6,8,9,11}, // whole tone half tone
}; 

void scls_chDispEnc(int val){
  switch(dispEncMode){
    case 0:
      switch(dispEncFnc[scls_opMode]){
        case 0:  
          if(rootNote + val < 0)rootNote = 12;
          rootNote=(rootNote + val)%12;
          break;
        case 1:
          if(scls_sclSel + val < 0)scls_sclSel = nScales;
          scls_sclSel=(scls_sclSel + val)%nScales;
          break;
        case 2:
          if(scls_sclStp + val < 0)scls_sclStp = scls_numSclStp[scls_sclSel];
          scls_sclStp=(scls_sclStp + val)%scls_numSclStp[scls_sclSel];
          break;
        case 3:
          if(scls_sclClr + val < 0)scls_sclClr = 1;
          scls_sclClr=(scls_sclClr + val)%2;
          break;
        case 4:
          if (shift==0)saveSong(genSq_actSng);
          if (shift==1)saveSong(0);
          if(genSq_actSng + val < 0)genSq_actSng = genSq_nSngs-1;
          genSq_actSng=(genSq_actSng + val)%genSq_nSngs;
          loadSong(genSq_actSng);
          break;
        case 5:
          if(scls_fledSrc + val < 0)scls_fledSrc = 0;
          scls_fledSrc=(scls_fledSrc + val)%4;
          break;
      }
      break;
    case 1:
      if(dispEncFnc[scls_opMode] + val < 0)dispEncFnc[scls_opMode] = nDispEncFnc[scls_opMode];
      dispEncFnc[scls_opMode]=(dispEncFnc[scls_opMode] + val)%nDispEncFnc[scls_opMode];
//      Serial.print("dispEncFnc[opMode] ");
//      Serial.println(dispEncFnc[opMode]);
      break;
  }
}

void scls_updDisp(){
  disp_Str(3, 10, "src:");
  disp_Int(31, 10, scls_fledSrc);
  disp_Str(45, 10, "sng:");
  disp_Int(73, 10, genSq_actSng);
  disp_Str(93, 10, "clr:");
  disp_Int(115, 10, scls_sclClr);


  switch (dispEncFnc[scls_opMode]){
    case 0:
      disp_RFrm(0, 23, 21, 19, 3);
      break;
    case 1:
      disp_RFrm(21, 23, 90, 19, 3);
      break;
    case 2:
      disp_RFrm(111, 23, 17, 19, 3);
      break;
    case 3:
      disp_RFrm(111, 0, 17, 19, 3);
      break;
    case 4:
      disp_RFrm(69, 0, 17, 19, 3);
      break;
    case 5:
      disp_RFrm(26, 0, 17, 19, 3);
      break;
  }

  disp_Str(3, 55, toneNm[tuning[5] % 12]);
  disp_Str(24, 55, toneNm[tuning[4] % 12]);
  disp_Str(45, 55, toneNm[tuning[3] % 12]);
  disp_Str(66, 55, toneNm[tuning[2] % 12]);
  disp_Str(87, 55, toneNm[tuning[1] % 12]);
  disp_Str(108, 55, toneNm[tuning[0] % 12]);

  //dispStr(3, 34, "Rt:");
  disp_Str(3, 34, toneNm[rootNote % 12]);
  disp_Str(24, 34, sclNm[scls_sclSel]);
  disp_Int(115, 34, scls_sclStp+1);
  
  //dispStr(20,40,"helleo");
  //dispInt(100, 40, 6);
}

void scls_updFleds(){
  float sumPix;
  int nDispFrts;
  
  if(opMode==scls_opMode)nDispFrts=nFrets+1;
  if(opMode!=scls_opMode)nDispFrts=genSq_pttnMOff;
  
  scls_dispScale();
  if(scls_fledSrc==0){
    for(int fret=0;fret<nLedFrets;fret++){
      for(int i=0;i<nStrings;i++){  
        scls_midiPix[i][fret][0]=0;
        scls_midiPix[i][fret][1]=0;
        scls_midiPix[i][fret][2]=0;
      }
    }
    scls_dispStrNotes();
    scls_dispMidi();
  }
  if(scls_fledSrc>0)scls_dispSq(scls_fledSrc-1);
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<nDispFrts;f++){
      for(int ch=0; ch < 3; ch++){
        trgtC[s][f][ch]=scls_sclPix[s][f][ch];
        sumPix = sumPix + scls_midiPix[s][f][ch];
      }
      if(sumPix>0)for(int ch=0; ch < 3; ch++){
        trgtC[s][f][ch]=scls_midiPix[s][f][ch];
      }
      sumPix=0;
    }
  }     
}

void scls_dispScale(){
  bool color=scls_sclClr;
  float brightS=0.5; //brighness off the colored scale
  float brightM=0.3; //brightness of the monocrome scale

  for(int s=0;s<nStrings;s++){ 
      for(int f=0;f<nLedFrets;f++){
        scls_sclPix[s][f][0]=0;
        scls_sclPix[s][f][1]=0;
        scls_sclPix[s][f][2]=0; 
      }
    }

  for(int i=0;i<scls_numSclStp[scls_sclSel];i++){
    for(int s=0;s<nStrings;s++){ 
      for(int f=0;f<nLedFrets;f++){
        byte actPitch=(tuning[s]+f+12)%12;
        byte trnPitch=(tuning[s]+f+24-rootNote+scls_scls[scls_sclSel][scls_sclStp])%12;
        if(scls_scls[scls_sclSel][i]==trnPitch){
          if(color==1){
            scls_sclPix[s][f][0]=tnClrs[actPitch][0]*brightS;
            scls_sclPix[s][f][1]=tnClrs[actPitch][1]*brightS;
            scls_sclPix[s][f][2]=tnClrs[actPitch][2]*brightS;
          }
          if(color==0){
            scls_sclPix[s][f][0]=brightM;
            scls_sclPix[s][f][1]=brightM;
            scls_sclPix[s][f][2]=brightM;
          }                  
        }   
      }       
    }
  }
}

void scls_dispMidi(){
  float bright=1;
  int midiTones[12];
  int midiNotes[127];
  for(int n=0;n<12;n++){
    midiTones[n]=0;
  }
  for(int n=0;n<127;n++){
    midiNotes[n]=0;
  }
  for(int ch=15;ch<=16;ch++){
    for(int n=0;n<=127;n++){
      if(extNotes[ch][n]>0){
        if(ch == 15)midiTones[n%12]=extNotes[ch][n];
        if(ch == 16)midiNotes[n]=extNotes[ch][n];
      }
    }
  }
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      //int idx=(note-rootNote)%12;
      int idx=note%12;
//      scls_midiPix[i][fret][0]=0;
//      scls_midiPix[i][fret][1]=0;
//      scls_midiPix[i][fret][2]=0;
      if(midiNotes[note]>0){
        bright=midiNotes[note]/127.0;
        scls_midiPix[i][fret][0]=tnClrs[idx][0]*bright;
        scls_midiPix[i][fret][1]=tnClrs[idx][1]*bright;
        scls_midiPix[i][fret][2]=tnClrs[idx][2]*bright;
      }      
      if(midiTones[note%12]>0){
        bright=midiTones[note%12]/127.0;
        scls_midiPix[i][fret][0]=tnClrs[idx][0]*bright;
        scls_midiPix[i][fret][1]=tnClrs[idx][1]*bright;
        scls_midiPix[i][fret][2]=tnClrs[idx][2]*bright;
      }
    }
  }
}

void scls_dispSq(int inst){
  float bright=1;
  int midiTones[12];
  int midiNotes[127];
  for(int n=0;n<12;n++){
    midiTones[n]=0;
  }
  for(int n=0;n<127;n++){
    midiNotes[n]=0;
  }
  for(int s=0;s<nStrings;s++){
    for(int n=0;n<=127;n++){
      if(genSq_actNotes[inst][s][n]>0){
        midiTones[n%12]=genSq_actNotes[inst][s][n];
        //if(ch == 16)midiNotes[n]=extNotes[ch][n];
      }
    }
  }
  
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      //int idx=(note-rootNote)%12;
      int idx=note%12;
      scls_midiPix[i][fret][0]=0;
      scls_midiPix[i][fret][1]=0;
      scls_midiPix[i][fret][2]=0;
      if(midiNotes[note]>0){
        bright=midiNotes[note]/127.0;
        scls_midiPix[i][fret][0]=tnClrs[idx][0]*bright;
        scls_midiPix[i][fret][1]=tnClrs[idx][1]*bright;
        scls_midiPix[i][fret][2]=tnClrs[idx][2]*bright;
      }      
      if(midiTones[note%12]>0){
        bright=midiTones[note%12]/127.0;
        scls_midiPix[i][fret][0]=tnClrs[idx][0]*bright;
        scls_midiPix[i][fret][1]=tnClrs[idx][1]*bright;
        scls_midiPix[i][fret][2]=tnClrs[idx][2]*bright;
      }
    }
  }
}

void scls_dispStrNotes(){
  float bright=1;
  int strNotes[nStrings][128];

  for(int s=0;s<nStrings;s++){
    for(int n=0;n<=127;n++){
      //strNotes[s][n]=0;
      strNotes[s][n]=extNotes[nStrings-s][n];
    }
  }
  
  for(int fret=0;fret<nLedFrets;fret++){
    for(int s=0;s<nStrings;s++){  
      int note=tuning[s]+fret;
      int cIdx=note%12;
//      scls_midiPix[s][fret][0]=0;
//      scls_midiPix[s][fret][1]=0;
//      scls_midiPix[s][fret][2]=0;
      if(strNotes[s][note]>0){
        bright=strNotes[s][note]/127.0;
        scls_midiPix[s][fret][0]=tnClrs[cIdx][0]*bright;
        scls_midiPix[s][fret][1]=tnClrs[cIdx][1]*bright;
        scls_midiPix[s][fret][2]=tnClrs[cIdx][2]*bright;
      }      
    }
  }
}
