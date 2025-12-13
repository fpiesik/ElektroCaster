void sndClock(long clck){
}
 
void sndNote(byte pitch, byte vel){
  Serial1.write(201);
  delayMicroseconds(waitS);
  Serial1.write(pitch);
  delayMicroseconds(waitS);
  Serial1.write(vel);
  delayMicroseconds(waitS);
}
//
void sndNotes(){
  for(byte n = 12;n<=72;n++){
    if(noteState[n]!=lastNoteState[n]){
      sndNote(n,noteState[n]);
//      if(noteState[i]==0){
//        sndNote(i,noteState[n]);
//        sndNote(i,noteState[n]);
//      }
      lastNoteState[n]=noteState[n];  
    }
  }
}

void sndCC(){
  for(byte i = 0;i<=127;i++){
    if(ccState[i]!=lastCCState[i]){
      Serial1.write(205);
      delayMicroseconds(waitS);
      Serial1.write(i);
      delayMicroseconds(waitS);
      Serial1.write(ccState[i]);
      delayMicroseconds(waitS);
      lastCCState[i]=ccState[i];  
    }
  }
}

void sndStrP(){
  for(byte i = 0;i<nStrings;i++){
    if(strP[i]!=lastStrP[i]){
      Serial1.write(206);
      delayMicroseconds(waitS);
      Serial1.write(i);
      delayMicroseconds(waitS);
      Serial1.write(int(strP[i]));
      delayMicroseconds(waitS);
      Serial1.write(int(100*(strP[i]-int(strP[i]))));
      delayMicroseconds(waitS);
      lastStrP[i]=strP[i];  
    }
  }
}

void sndStrA(){
  for(byte i = 0;i<nStrings;i++){
    if(strA[i]!=lastStrA[i]){
      Serial1.write(207);
      delayMicroseconds(waitS);
      Serial1.write(i);
      delayMicroseconds(waitS);
      Serial1.write(int(100*(strA[i])));
      delayMicroseconds(waitS);
      lastStrA[i]=strA[i];  
    }
  }
}
