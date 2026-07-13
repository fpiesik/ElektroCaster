void updLedFrets(){
  if (millis()-fled_frameTimer > fled_frameInt){
    ledsTrgOff();
    switch (opMode){
      case strSetup_opMode:
        strSetup_updFleds();
        break;
      case strArp_opMode:
        if(fbrdMode==0&&fbrdSeqVHld==0)scls_updFleds();
        if(fbrdMode==1||fbrdSeqVHld==1)strArp_updFleds();
        genSq_updPttnFleds();
        break;
      case genSq_opMode:
        if(fbrdMode==0&&fbrdSeqVHld==0)scls_updFleds();
        if(fbrdMode==1||fbrdSeqVHld==1)genSq_updFleds();
        genSq_updPttnFleds();
        break;
      case genSq_opMode+1:
        if(fbrdMode==0&&fbrdSeqVHld==0)scls_updFleds();
        if(fbrdMode==1||fbrdSeqVHld==1)genSq_updFleds();
        genSq_updPttnFleds();
        break;
      case genSq_opMode+2:
        if(fbrdMode==0&&fbrdSeqVHld==0)scls_updFleds();
        if(fbrdMode==1||fbrdSeqVHld==1)genSq_updFleds();
        genSq_updPttnFleds();
        break;
    }
    
  
    lightSlew();
    pixels();
    if(ledsChngd()==1){
      FastLED.show();
    }
    fled_frameTimer=millis();
  }
}

void genSq_updPttnFleds(){ 
  genSq_pttnDrwGrid();
  genSq_pttnDrwStat();
  for(int s=0;s<nStrings;s++){
    for(int p=0;p<genSq_nPttn/2;p++){
      for(int ch=0;ch < 3; ch++){
        trgtC[s][p+genSq_pttnMOff][ch]=genSq_pttnGridPix[s][p][ch]+genSq_pttnPttnPix[s][p][ch];
      }
    }
  }        
}

void genSq_pttnDrwGrid(){ 
  int inst=genSq_actInst;
  for(int i=0;i<genSq_nInst;i++){
    float brght;
    if(inst!=genSq_nInst-i-1)brght=1;
    if(inst==genSq_nInst-i-1)brght=5;
    for(int s=0;s<nStrings;s++){
      for(int f=0;f<genSq_nPttn/2;f++){
        for(int c=0;c<3;c++){  
          if(s==i*2+1||s==i*2)genSq_pttnGridPix[s][f][c]=genSq_gridColor[genSq_nInst-i-1][c]*brght;
        }
      }
    }
  }
}

void genSq_pttnDrwStat(){ 
  for(int s = 0;s<nStrings;s++){
    for(int f = 0;f<genSq_nPttn/2;f++){
      for(int c = 0;c<3;c++){
        genSq_pttnPttnPix[s][f][c]=0;
      }
    }
  }
  for(int i=0;i<genSq_nInst;i++){
    int actPttn=genSq_actPttn[i];
    int edtPttn=genSq_edtPttn[i];
    if(i==0 && strArp_modeSel){
      int arpPttn=11-strArp_modeVal;
      if(arpPttn<0)arpPttn=0;
      if(arpPttn>=genSq_nPttn)arpPttn=genSq_nPttn-1;
      actPttn=arpPttn;
      edtPttn=arpPttn;
    }
    int actStr=i*2+actPttn/(genSq_nPttn/2);
    int edtStr=i*2+edtPttn/(genSq_nPttn/2);
    int actFrt=actPttn-genSq_nPttn/2*(actPttn/(genSq_nPttn/2));
    int edtFrt=edtPttn-genSq_nPttn/2*(edtPttn/(genSq_nPttn/2));
    for(int c=0;c<3;c++){  
      float edtVal=0.07;
      float actVal=0.25;
      if(i==0 && strArp_modeSel && c!=2)edtVal=0, actVal=0;
      genSq_pttnPttnPix[nStrings-1-edtStr][edtFrt][c]=edtVal;
      genSq_pttnPttnPix[nStrings-1-actStr][actFrt][c]=actVal;
    }
  }
}

void lightSlew(){
  float slew=0.3; //smaller=faster
  float thrsh=0.01;
  for(int s=0;s<nStrings;s++){  
    for(int f=0;f<nLedFrets;f++){
      if(trgtC[s][f][0]<actC[s][f][0])actC[s][f][0]=actC[s][f][0]*slew;
      if(actC[s][f][0]<=thrsh)actC[s][f][0]=0;
      if(trgtC[s][f][0]>=actC[s][f][0])actC[s][f][0]=trgtC[s][f][0];

      if(trgtC[s][f][1]<actC[s][f][1])actC[s][f][1]=actC[s][f][1]*slew;
      if(actC[s][f][1]<=thrsh)actC[s][f][1]=0;
      if(trgtC[s][f][1]>=actC[s][f][1])actC[s][f][1]=trgtC[s][f][1];

      if(trgtC[s][f][2]<actC[s][f][2])actC[s][f][2]=actC[s][f][2]*slew;
      if(actC[s][f][2]<=thrsh)actC[s][f][2]=0;
      if(trgtC[s][f][2]>=actC[s][f][2])actC[s][f][2]=trgtC[s][f][2];

      //pixel(s,f,actC[s][f]);
    }
  }
}

void pixel(byte s, byte f, float c[3]){
  for(int ch=0;ch<3;ch++){
    if(c[ch]>1.0)c[ch]=1.0;
  }
  frtPix[led_pixPos[s][f]]=CRGB(c[2]*fled_blueC, c[0]*fled_redC, c[1]*fled_greenC);
}

void pixels(){
  for(int s=0;s<nStrings;s++){  
    for(int f=0;f<nLedFrets;f++){
      pixel(s,f,actC[s][f]);
    }
  }
}

//void sndPixel(byte s, byte f, float c[3]){
//  int sC[3];
//  sC[0]=c[0]*fled_redC;
//  sC[1]=c[1]*fled_greenC;
//  sC[2]=c[2]*fled_blueC;
//  for(int ch=0;ch<3;ch++){
//    if(c[ch]>250)c[ch]=250;
//  }
//  Serial5.write(254);
//  //delayMicroseconds(waitS);
//  Serial5.write(s);
//  Serial5.write(f);
//  Serial5.write(sC[0]);
//  Serial5.write(sC[1]);
//  Serial5.write(sC[2]);
//}
//void sndPixels(){
//  for(int s=0;s<nStrings;s++){  
//    for(int f=0;f<nLedFrets;f++){
//      sndPixel(s,f,actC[s][f]);
//    }
//  }
//  Serial5.write(255);
//}

bool ledsChngd(){
  bool chngd=0;
  for(int i=0; i<NUMPIXELS; i++){
    if(frtPix[i]!=lastFrtPix[i]){
      lastFrtPix[i]=frtPix[i];
      chngd=1;
    }    
  }
  return chngd;
}

void fled_dispPixel(byte str,byte fret,float rgb[3],float bright){
  trgtC[str][fret][0]=rgb[0]*bright;
  trgtC[str][fret][1]=rgb[1]*bright;
  trgtC[str][fret][2]=rgb[2]*bright;
}


void mkColors(){
  for(int i=0;i<12;i++){
    HSBtoRGB(float(i/12.0));
    tnClrs[i][0]=hsbRGB[0];
    tnClrs[i][1]=hsbRGB[1];
    tnClrs[i][2]=hsbRGB[2];
  }
}

void HSBtoRGB(float hue) {
   float red = 0;
   float green = 0;
   float blue = 0;
   float thrRG =0.5;
   float thrGB =0.75;
   if(hue>=1.0)hue=0.99;
    if (hue<thrRG){
      red = 1.0-hue/thrRG;
      green = hue/thrRG;
      blue = 0;
    }
    if (hue>=thrRG&&hue<thrGB){
      red=0;
      green = 1-(hue-thrRG)/(thrGB-thrRG);
      blue = (hue-thrRG)/(thrGB-thrRG);
      
    }
    if (hue>=thrGB){
      red = (hue-thrGB)/(1-thrGB);
      green = 0;
      blue = 1-(hue-thrGB)/(1-thrGB);
    }
   
   hsbRGB[0]=red;
   hsbRGB[1]=green;
   hsbRGB[2]=blue;
}

void fled_clr(){
  for(int s=0;s<nStrings;s++){  
    for(int f=0;f<nLedFrets;f++){
      //frtPix[led_pixPos[s][f]]=CRGB(0,0,0);
    }
  }
}

void ledsTrgOff(){
  for(int s=0;s<nStrings;s++){  
    for(int f=0;f<nLedFrets;f++){
      trgtC[s][f][0]=0;
      trgtC[s][f][1]=0;
      trgtC[s][f][2]=0;
    }
  }
}
