void loop(void){
  usbMIDI.read();

  if (inbound.parseStream( &Serial5 ) ) {
    
    if (inbound.fullMatch("opMode") ) {
      chOpMode(inbound.nextByte());
    }
    if (inbound.fullMatch("dispMode") ) {
      chDispMode(inbound.nextByte());
    }
    if (inbound.fullMatch("kickMode") ) {
      chKickMode(inbound.nextByte());
    }
    if (inbound.fullMatch("bowMode") ) {
      chBowMode(inbound.nextByte());
    }
    if (inbound.fullMatch("bowOn") ) {
      bowOn=(inbound.nextByte());
    }
    
    if (inbound.fullMatch("seqHit")) {
      byte str=inbound.nextByte();
      float val=inbound.nextFloat();
      seqHit(str, val);
    }
    if (inbound.fullMatch("strFret")) {
      byte str=inbound.nextByte();
      float val=inbound.nextFloat();
      strFret(str, val);
    }
    if (inbound.fullMatch("env1")) {
      byte para=inbound.nextByte();
      float val=inbound.nextFloat();
      valEnv1[para]=scale(val,0,sclEnv1[para]);
      chEnv1(para,valEnv1[para]);
    }
    if (inbound.fullMatch("env2")) {
      byte para=inbound.nextByte();
      float val=inbound.nextFloat();
      valEnv2[para]=scale(val,0,sclEnv2[para]);
      chEnv2(para,valEnv2[para]);
    }
    if (inbound.fullMatch("fil")) {
      byte para=inbound.nextByte();
      float val=inbound.nextFloat();
      valFilter[para]=scale(val,1,sclFilter[para]);
      chFilter(para,valFilter[para]);
    }
    if (inbound.fullMatch("vol")) {
      float val=inbound.nextFloat();
      ampOut.gain(val*val);
    }
    if (inbound.fullMatch("c")) {
      usbMIDI.sendRealTime(usbMIDI.Clock);
    }
    if (inbound.fullMatch("start")) {
      usbMIDI.sendRealTime(usbMIDI.Start);
    }
    if (inbound.fullMatch("stop")) {
      usbMIDI.sendRealTime(usbMIDI.Stop);
    }
    if (inbound.fullMatch("del")) {
      byte para=inbound.nextByte();
      float val=inbound.nextFloat();
      valDelay[para]=scale(val,1,sclDelay[para]);
      chDelay(para,valDelay[para]);
    }
    if (inbound.fullMatch("lfo1")) {
      byte para=inbound.nextByte();
      float val=inbound.nextFloat();
      valLfo1[para]=scale(val,1,sclLfo1[para]);
      chLfo1(para,valLfo1[para]);
    }                   
    if (inbound.fullMatch("g")) {
      sndNotes();
    }   
    if (inbound.fullMatch("cc")) {
      byte para=inbound.nextByte();
      byte val=inbound.nextByte();
      usbMIDI.sendControlChange(para, val, 1);
    }
    if (inbound.fullMatch("trnsp")) {
      byte para=inbound.nextByte();
      byte val=inbound.nextByte();
      if(val==1)usbMIDI.sendNoteOn(2, 127, 16);
      if(val==0)usbMIDI.sendNoteOff(2, 0, 16);
    }
 }
       
if(bowOn!=lastBowOn){
for (int i=0; i<nStrings; i++) {
    if(bowOn==0)amps[i]->gain(0);
    if(bowOn==1&&strState[i]>0)amps[i]->gain(50);
  }
  lastBowOn=bowOn;
}
   
if(1) {
  if(millis() - last_time >= 3000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");    
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");    
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    last_time = millis();
  }
}
}
