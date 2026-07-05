//displays for the different opModes

void strSetup_chDispEnc(int val){
  if(dispEncFnc[strSetup_opMode] + val < 0)dispEncFnc[strSetup_opMode] = nDispEncFnc[strSetup_opMode];
  dispEncFnc[strSetup_opMode]=(dispEncFnc[strSetup_opMode] + val)%nDispEncFnc[strSetup_opMode];
//Serial.print("dispEncFnc[opMode] ");
//Serial.println(dispEncFnc[opMode]);
}

void strSetup_chStrEnc(byte s, int val){
  switch(dispEncFnc[strSetup_opMode]){
    case 0:
      tuning[s] = tuning[s] + val;
      break;
    case 1:
      strGain[s] = strGain[s] + val;
      if(strGain[s]<0)strGain[s]=0;
      if(strGain[s]>strGainMx)strGain[s]=strGainMx;
      sndStrGain(s,strGain[s]);
      break;
    case 2:
      bpm=bpm+val;
        if(bpm<1)bpm=1;
        if(bpm>250)bpm=250;
        chngBpm(bpm);
      break;
  }
}


void strSetup_updDisp(){
  disp_Str(3, 8, "strSetup");
  disp_Str(75, 8, "bpm:");
  disp_Int(103, 8, bpm);

//switch (dispEncFnc){
//  case 0:
//    disp_RFrm(0, 23, 21, 19, 3);
//    break;
//  case 1:
//    disp_RFrm(21, 23, 90, 19, 3);
//    break;
//  case 2:
//    disp_RFrm(111, 23, 17, 19, 3);
//    break;
//}

  disp_Str(3, 55, toneNm[tuning[5] % 12]);
  disp_Str(24, 55, toneNm[tuning[4] % 12]);
  disp_Str(45, 55, toneNm[tuning[3] % 12]);
  disp_Str(66, 55, toneNm[tuning[2] % 12]);
  disp_Str(87, 55, toneNm[tuning[1] % 12]);
  disp_Str(108, 55, toneNm[tuning[0] % 12]);

  disp_Int(3, 34, strGain[5]);
  disp_Int(24, 34, strGain[4]);
  disp_Int(45, 34, strGain[3]);
  disp_Int(66, 34, strGain[2]);
  disp_Int(87, 34, strGain[1]);
  disp_Int(108, 34, strGain[0]);

  switch (dispEncFnc[strSetup_opMode]){
    case 0:
      disp_RFrm(0, 46, 125, 15, 3);
      break;
    case 1:
      disp_RFrm(0, 25, 125, 15, 3);
      break;
    case 2:
    disp_RFrm(0, 0, 125, 15, 3);
      break;
  }
  
  //dispStr(3, 34, "Rt:");
  //disp_Str(3, 34, toneNm[rootNote % 12]);
  //disp_Str(24, 34, sclNm[scls_sclSel]);
  //disp_Int(115, 34, scls_sclStp+1);
  //dispStr(20,40,"helleo");
  //dispInt(100, 40, 6);
}

void strSetup_updFleds(){
  float inTnWin=0.08;
  for(int s=0;s<nStrings;s++){
    //byte fret=10*(strP[s]-tuning[s])+12.05;
    byte fret=strP[s]-tuning[s]+12.5;
    //int cIdx=int(strP[s]*10)%120;
    int cIdx=int(strP[s]+0.5)%12;
    float diver=abs(strP[s]-int(strP[s]+0.5))*3;
    float diverB=(strP[s]-int(strP[s]+0.5))*3;
    if(diver>1)diver=1;

//    trgtC[s][12][0]=tnClrs[tuning[s]%12][0];
//    trgtC[s][12][1]=tnClrs[tuning[s]%12][1];
//    trgtC[s][12][2]=tnClrs[tuning[s]%12][2];
    
    trgtC[s][12][0]=0.5;
    trgtC[s][12][1]=0.5;
    trgtC[s][12][2]=0.5;
    
//    trgtC[s][12][0]=tnClrs[tuning[s]%12][0];
//    trgtC[s][12][1]=tnClrs[tuning[s]%12][1];
//    trgtC[s][12][2]=tnClrs[tuning[s]%12][2];
    
    trgtC[s][0][0]=tnClrs[tuning[s]%12][0];
    trgtC[s][0][1]=tnClrs[tuning[s]%12][1];
    trgtC[s][0][2]=tnClrs[tuning[s]%12][2];
    
    
    if(strA[s]>0.03){
      if(diverB>inTnWin){
        trgtC[s][fret+1][0]=diver-inTnWin;
        trgtC[s][fret+1][1]=diver-inTnWin;
        trgtC[s][fret+1][2]=diver-inTnWin;
      }
      if(diverB<-inTnWin){
        trgtC[s][fret-1][0]=diver-inTnWin;
        trgtC[s][fret-1][1]=diver-inTnWin;
        trgtC[s][fret-1][2]=diver-inTnWin;
      }
//
//          if(strA[s]>0.03){
//      if(diff>0.05 && diff<0.5){
//        trgtC[s][13][0]=diver;
//        trgtC[s][13][1]=diver;
//        trgtC[s][13][2]=diver;
//      }
//      if(diff<-0.05 && diff>-0.5){
//        trgtC[s][11][0]=diver;
//        trgtC[s][11][1]=diver;
//        trgtC[s][11][2]=diver;
//      }
      
//      trgtC[s][fret][0]=tnClrs[cIdx][0]*(1-diver)+diver/2;
//      trgtC[s][fret][1]=tnClrs[cIdx][1]*(1-diver)+diver/2;
//      trgtC[s][fret][2]=tnClrs[cIdx][2]*(1-diver)+diver/2;    

      trgtC[s][fret][0]=tnClrs[cIdx][0];
      trgtC[s][fret][1]=tnClrs[cIdx][1];
      trgtC[s][fret][2]=tnClrs[cIdx][2]; 
    }
  }
}
