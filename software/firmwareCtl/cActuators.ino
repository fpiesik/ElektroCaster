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

//fire the multiple kick at once in a short sequence to prevent voltage drop
void cueKicks(){
    for(int s=0;s<nStrings;s++){
      if(kickCue[s]==1){
        kickup(s);
        kickCue[s]=0;
        s=nStrings;
      }
    }
    kCueTimer=millis();
}
    


//void kickOff1(){
//digitalWrite(kickupPins[0],0);
//kTimer1.end();
//}
//void kickOff2(){
//digitalWrite(kickupPins[1],0);
//kTimer2.end();
//}
//void kickOff3(){
//digitalWrite(kickupPins[2],0);
//kTimer3.end();
//}
//void kickOff4(){
//digitalWrite(kickupPins[3],0);
//kTimer4.end();
//}
//void kickOff5(){
//digitalWrite(kickupPins[4],0);
//kTimer5.end();
//}
//void kickOff6(){
//digitalWrite(kickupPins[5],0);
//kTimer6.end();
//}
