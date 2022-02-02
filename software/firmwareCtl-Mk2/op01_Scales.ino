int scls_scls[nScales][12]={
{}, //off
{0}, //Root
{0,2,4,5,7,9,11}, //ionian
{0,3,5,7,10}, //pentatonic
{0,1,4,5,7,8,10}, // mixo b9 b13
{0,2,4,6,8,10}, // whole tone
{0,2,3,5,6,8,9,11}, // whole tone half tone
{0,1,3,4,6,8,10} // altered
}; 

void scls_chDispEnc(int val){
  switch(dispEncMode){
    case 0:
      switch(dispEncFnc[opMode]){
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

void scls_updDisp(){
  disp_Str(3, 8, "scales");

  switch (dispEncFnc[opMode]){
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
  disp_Int(115, 8, scls_sclClr);
  //dispStr(20,40,"helleo");
  //dispInt(100, 40, 6);
}

void scls_updFleds(){
  float sumPix;
  scls_dispScale();
  scls_dispMidi();
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<nLedFrets;f++){
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
  float brightS=0.3; //brighness off the colored scale
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
  for(int n=0;n<12;n++){
    midiTones[n]=0;
  }
  for(int n=0;n<127;n++){
    if(extNotes[n]>0)midiTones[n%12]=extNotes[n];
  }
  for(int fret=0;fret<nLedFrets;fret++){
    for(int i=0;i<nStrings;i++){  
      byte note=tuning[i]+fret;
      //int idx=(note-rootNote)%12;
      int idx=note%12;
      if(midiTones[note%12]==0){
        scls_midiPix[i][fret][0]=0;
        scls_midiPix[i][fret][1]=0;
        scls_midiPix[i][fret][2]=0;
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
