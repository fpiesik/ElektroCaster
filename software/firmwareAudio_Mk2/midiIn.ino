void rcvNoteOn(byte channel, byte note, byte velocity) {
  if(channel==midiCh){
    noteState[note]=velocity;
    sndNote(note,velocity);
  }
  if(channel!=midiCh){
    for(int i=0;i<200;i++){
      int dosmth=dosmth+1;
    }
  }
}

void rcvNoteOff(byte channel, byte note, byte velocity) {
  if(channel==midiCh){
    noteState[note]=0;
    sndNote(note,0);
  }
}

float scale(float val,byte op, float scl){
  if(op==0)return(val*scl);
  if(op==1)return(val*val*scl);
  return (0);
}

void rcvCC(byte ch, byte cc, byte data) {
  ccState[cc]=data;
  sndCC();
  float val=data/127.0; 
  
  for(int n=0;n<4;n++){
    if(cc==ccFilter[n]){
      valFilter[n]=scale(val,0,sclFilter[n]);
      chFilter(n,valFilter[n]);
    }
  }

  for(int n=0;n<7;n++){
    if(cc==ccEnvA[n]){
      float sclVal=scale(val,1,sclEnvA[n]);
      chEnvA(n,sclVal);
    }
    if(cc==ccEnvF[n]){
      float sclVal=scale(val,1,sclEnvF[n]);
      chEnvF(n,sclVal);
    }
  }
}
