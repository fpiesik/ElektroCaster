void kickup(byte s){
 if (kickOn==1&&kickMode>1){
  if(s==0){
    Timer1.stop();
    digitalWrite(kickupPins[s],1);
    Timer1.start(kickDur[s]);
  }
  if(s==1){
    Timer2.stop();
    digitalWrite(kickupPins[s],1);
    Timer2.start(kickDur[s]);
  }
  if(s==2){
    Timer7.stop();
    digitalWrite(kickupPins[s], 1);
    Timer7.start(kickDur[s]);
  }
  if(s==3){
    Timer4.stop();
    digitalWrite(kickupPins[s], 1);
    Timer4.start(kickDur[s]);
  }
  if(s==4){
    Timer5.stop();
    digitalWrite(kickupPins[s], 1);
    Timer5.start(kickDur[s]);
  }
  if(s==5){
    Timer6.stop();
    digitalWrite(kickupPins[s], 1);
    Timer6.start(kickDur[s]);
  }
 } 
}


void kickOff1(){
digitalWrite(kickupPins[0],0);
Timer1.stop();
}
void kickOff2(){
digitalWrite(kickupPins[1],0);
Timer2.stop();
}
void kickOff3(){
digitalWrite(kickupPins[2],0);
Timer7.stop();
}
void kickOff4(){
digitalWrite(kickupPins[3],0);
Timer4.stop();
}
void kickOff5(){
digitalWrite(kickupPins[4],0);
Timer5.stop();
}
void kickOff6(){
digitalWrite(kickupPins[5],0);
Timer6.stop();
}
