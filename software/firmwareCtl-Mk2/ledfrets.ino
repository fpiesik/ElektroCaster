
void updLedFrets(){
  if (millis()-fled_frameTimer > fled_frameInt){
    ledsTrgOff();
    switch (fledMode){
      case 0:
        strSetup_updFleds();
        break;
      case 1:
        scls_updFleds();
        break;
      case 2:
        strArp_updFleds();
        break;
      case 3:
        drmSq_updFleds();
        break;
    }
    lightSlew();
    if(ledsChngd()==1)FastLED.show();
    fled_frameTimer=millis();
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

      pixel(s,f,actC[s][f]);
    }
  }
}

void pixel(byte s, byte f, float c[3]){
  for(int ch=0;ch<3;ch++){
    if(c[ch]>1.0)c[ch]=1.0;
  }
  frtPix[led_pixPos[s][f]]=CRGB(c[2]*fled_blueC, c[0]*fled_redC, c[1]*fled_greenC);
}

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
      frtPix[led_pixPos[s][f]]=CRGB(0,0,0);
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
