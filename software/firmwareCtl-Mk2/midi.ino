void setMidiHandles(){
  usbMIDI.setHandleClock(midiClock);
  usbMIDI.setHandleStart(midiStart);
  usbMIDI.setHandleStop(midiStop);
  usbMIDI.setHandleNoteOn(rcvNoteOn);
  usbMIDI.setHandleNoteOff(rcvNoteOff);
//  usbMIDI.setHandleControlChange(rcvCC);
}

void sndMidiNote(byte note,byte velocity, byte channel){
  if(velocity>0){
    usbMIDI.sendNoteOn(note, velocity, channel, 0);
  }
  if(velocity==0){
    usbMIDI.sendNoteOff(note, 0, channel, 0);
  }
//    Serial.print("note: ");
//    Serial.print(note);
//    Serial.print(" vel: ");
//    Serial.println(velocity);
}

void sndMidiCC(byte cc, byte val,byte ch){
  usbMIDI.sendControlChange(cc, val, ch);
}


void rcvNoteOn(byte channel, byte note, byte velocity) {
  if(channel==midiCh){
    extNotes[note]=velocity;
  }
}

void rcvNoteOff(byte channel, byte note, byte velocity) {
  if(channel==midiCh){
    extNotes[note]=0;
  }
}


//void rcvCC(byte ch, byte cc, byte data) {
//  ccState[cc]=data;
//  sndCC();
//  float val=data/127.0; 
//  
//  for(int n=0;n<4;n++){
//    if(cc==ccFilter[n]){
//      valFilter[n]=scale(val,0,sclFilter[n]);
//      chFilter(n,valFilter[n]);
//    }
//  }
//
//  for(int n=0;n<7;n++){
//    if(cc==ccEnvA[n]){
//      float sclVal=scale(val,1,sclEnvA[n]);
//      chEnvA(n,sclVal);
//    }
//    if(cc==ccEnvF[n]){
//      float sclVal=scale(val,1,sclEnvF[n]);
//      chEnvF(n,sclVal);
//    }
//  }
//}
