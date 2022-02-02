void kickup(byte s){
  switch(s){
    case 0:
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      break;
      
    case 1:
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      break;
    case 2:
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      break;
    case 3:
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      break;
    case 4:
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      break;
    case 5:
      digitalWrite(kickupPins[s],1);
      kickState[s]=1;
      kickTimer[s]=millis();
      break;
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

//fire the multiple kick that occure at once in a timed sequence to prevent voltage drop
void cueKicks(){
  kickOff();
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

void kick(byte str){
  if(kickOn==1)kickCue[str]=1;
}
