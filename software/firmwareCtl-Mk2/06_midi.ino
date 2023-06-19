void setMidiHandles(){
    MIDI.setHandleClock(midiClock);
    MIDI.setHandleStart(midiStart);
    MIDI.setHandleStop(midiStop);
//  usbMIDI.setHandleNoteOn(rcvNoteOn);
//  usbMIDI.setHandleNoteOff(rcvNoteOff);
//  usbMIDI.setHandleControlChange(rcvCC);
  MIDI.setHandleNoteOn(rcvNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(rcvNoteOff);

}

void sndMidiNote(byte note,byte velocity, byte channel){
  if(velocity>0){
    //usbMIDI.sendNoteOn(note, velocity, channel, 0);
    MIDI.sendNoteOn(note, velocity, channel);
  }
  if(velocity==0){
    //usbMIDI.sendNoteOff(note, 0, channel, 0);
    MIDI.sendNoteOff(note, velocity, channel);
  }
//    Serial.print("note: ");
//    Serial.print(note);
//    Serial.print(" vel: ");
//    Serial.println(velocity);
}

void sndMidiNotePress(int str, int frt){
  static int lastNote[nStrings];
  static int lastChn[nStrings];
  int chn = mInst_chn;
  //int chn=genSq_chn[genSq_actInst][genSq_actPttns[genSq_actPttnsIdx][genSq_actInst]][str][genSq_strEncFnc_chn];
  if (frt > 0){ 
    int note=tuning[str]+frt;
    sndMidiNote(lastNote[str],0, chn);
    sndMidiNote(note,120, chn);
    lastNote[str]=note;
    lastChn[str]=chn;
  }    
  if (frt == 0){ 
    if(lastNote>0)sndMidiNote(lastNote[str],0, lastChn[str]);
    lastNote[str]=0;
  }
}

void sndMidiCC(byte cc, byte val,byte ch){
  //usbMIDI.sendControlChange(cc, val, ch);
  MIDI.sendControlChange(cc, val, ch);
}

void sndMidiClck(int state){
  if(extClk==0){
    if(state==0)MIDI.sendRealTime(midi::Start);
    if(state==1)MIDI.sendRealTime(midi::Clock);
    if(state==2)MIDI.sendRealTime(midi::Stop);
  }
}


void rcvNoteOn(byte channel, byte note, byte velocity) {
  extNotes[channel][note]=velocity;
  //midiKick(channel,velocity);
  
}

void rcvNoteOff(byte channel, byte note, byte velocity) {
  extNotes[channel][note]=0;
}

void midiClock(){ 
  if(extClk==1){
    mClock++;
    cmpClck(mClock);
    updDisplay();
  }
}

void midiStart() {
  mClock=-1;
  extClk=1;
  cmpClck(mClock);
}
void midiStop() {
  mClock = -1;
  extClk = 0;
  cmpClck(mClock);
  for(int ch=0;ch<=16;ch++){
    for(int n=0;n<=127;n++){
      extNotes[ch][n]=0;
    }
  }
  genSq_allNOff();
}

void genSq_allNOff(){
  for(int inst = 0; inst<genSq_nInst; inst++){
    for(int str = 0; str<nStrings; str++){
      for(int pttn = 0; pttn<genSq_nPttn; pttn++){
        genSq_sndStpOffNMW(inst, pttn, str);
      }
    }
  }
}

void midiKick(int chn, int v){
  if(chn<=nStrings){
    int s = nStrings-chn;
    sndTrigEnv(s,v/127.0);
    kick(s);
  }
  
//  for(int s=0;s<nStrings;s++){
//    if(n == tuning[s]){
//    sndTrigEnv(s,v/127.0);
//    kick(s);
//    }
//  }
  
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