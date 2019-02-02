 void sndClock(long idx){
  outbound.beginPacket("c");
  outbound.addLong(idx);
  outbound.streamPacket(&Serial5);
  delayMicroseconds(waitS);
}
 void sndFin(){
  outbound.beginPacket("f");
  outbound.streamPacket(&Serial5);
}
void sndNote(byte pitch, byte vel){
  outbound.beginPacket("note");
  outbound.addByte(pitch);
  outbound.addByte(vel);
  outbound.streamPacket(&Serial5);
  delayMicroseconds(waitS);
}

void sndNotes(){
  outbound.beginPacket("n");
  for(byte i = 0;i<41;i++){
    //if(noteState[i]!=lastNoteState[i]){
        outbound.addByte(noteState[i+35]);
        lastNoteState[i+35]=noteState[i+35];
    //)
  }
  outbound.streamPacket(&Serial5);
  sndFin();
}

void sndCC(byte cc, byte val){
  outbound.beginPacket("cc");
  outbound.addByte(cc);
  outbound.addByte(val);
  outbound.streamPacket(&Serial5);
  delayMicroseconds(waitS);}
  
void rcvNoteOn(byte channel, byte note, byte velocity) {
if(channel==1)noteState[note]=velocity;
}
void rcvNoteOff(byte channel, byte note, byte velocity) {
if(channel==1)noteState[note]=0;
}

float scale(float val,byte op, float scl){
  if(op==0)return(val*scl);
  if(op==1)return(val*val*scl);
}

void rcvCC(byte ch, byte cc, byte data) {
  sndCC(cc,data);
  float val=data/127.0; 
  
  for(int n=0;n<2;n++){
    if(cc==ccFilter[n]){
      valFilter[n]=scale(val,1,sclFilter[n]);
      chFilter(n,valFilter[n]);
    }
  }

  for(int n=0;n<6;n++){
    if(cc==ccEnv1[n]){
      valEnv1[n]=scale(val,0,sclEnv1[n]);
      chEnv1(n,valEnv1[n]);
    }
  }
}
