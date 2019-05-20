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
