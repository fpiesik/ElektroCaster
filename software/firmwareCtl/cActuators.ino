void kickup(byte s){
  if (1){
    if(s==0){
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      }
    if(s==1){
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      }
    if(s==2){
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      }
    if(s==3){
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      }
    if(s==4){
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      }
    if(s==5){
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      }
  }
}

void kickOff(){
  for(int s=0;s<nStrings;s++){
    if (millis()-kickTimer[s] >= kickDur[s] && kickState[s]==1){ 
      digitalWrite(kickupPins[s],0);
      kickState[s]=0;
      }
  }
}

//fire the multiple kick at once in a short sequence to prevent voltage drop
void cueKicks(){
  if (millis()-kCueTimer > kCueInt){ 
    for(int s=0;s<nStrings;s++){
      if(kickCue[s]==1){
        kickup(s);
        kickCue[s]=0;
        s=nStrings;
      }
    }
    kCueTimer=millis();
  }
}
