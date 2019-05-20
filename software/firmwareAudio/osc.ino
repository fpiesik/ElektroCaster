void sndClock(long clck){
}
 
void sndFin(){
//  Serial5.write(200);
}

void sndNote(byte pitch, byte vel){
  Serial5.write(201);
  delayMicroseconds(waitS);
  Serial5.write(pitch);
  delayMicroseconds(waitS);
  Serial5.write(vel);
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
