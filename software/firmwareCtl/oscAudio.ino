//void rcvNote(byte pitch, byte velocity){
//  float vel=velocity/127.0;
//  midiNotes[pitch]=vel;
//  }
//
//float scale(float val,byte op, float scl){
//  if(op==0)return(val*scl);
//  if(op==1)return(val*val*scl);
//}
//
//void rcvCC(byte cc, byte value){
//  float val=value/127.0;
//  for(int n=0;n<7;n++){
//    if(cc==ccTd[0]){
//      for(int s=0;s<nStrings;s++){
//        byte tdI=scale(val,0,sclTd);
//        tmDv[s]=tmDvs[tdI];
//      }
//    }
//    if(cc==ccTd[n]&&n!=0){
//      byte tdI=scale(val,0,sclTd);
//      tmDv[n-1]=tmDvs[tdI];
//    }
//  }
//  
//  for(int n=0;n<7;n++){
//    if(cc==ccArpRpt[0]){
//      for(int s=0;s<nStrings;s++){
//        arpRpt[s]=1+scale(val,0,sclArpRpt);
//      }
//    }
//    if(cc==ccArpRpt[n]&&n!=0){
//      byte tdI=scale(val,0,sclTd);
//      arpRpt[n-1]=1+scale(val,0,sclArpRpt);
//    }
//  }
//  
//  if(cc==ccArpMode){
//    arpMode=scale(val,0,sclArpMode);
//  }
//  if(cc==ccArpClkMode){
//    arpClkMode=scale(val,0,sclArpClkMode);
//  }
//} 
//
//void strNote(byte str, float gain){
//  outbound.beginPacket("seqHit");
//  outbound.addByte(str);
//  outbound.addFloat(gain);
//  outbound.streamPacket(&Serial1);
//  delayMicroseconds(waitS);
//}
//
//void strFret(byte str, byte fret){
//  outbound.beginPacket("strFret");
//  outbound.addByte(str);
//  outbound.addByte(fret);
//  outbound.streamPacket(&Serial1);
//  delayMicroseconds(waitS);
//  //Serial.println(str);
//}
//
//void sndMode(byte mode){
//  outbound.beginPacket("opMode");
//  outbound.addByte(mode);
//  outbound.streamPacket(&Serial1);
//  delayMicroseconds(waitS);
//}
//
//void sndDispMode(byte mode){
//  outbound.beginPacket("dispMode");
//  outbound.addByte(mode);
//  outbound.streamPacket(&Serial1);
//  delayMicroseconds(waitS);
//}
//
//void sndKickMode(byte mode){
//  outbound.beginPacket("kickMode");
//  outbound.addByte(mode);
//  outbound.streamPacket(&Serial1);
//  delayMicroseconds(waitS);
//}
//
//void sndBowMode(byte mode){
//  outbound.beginPacket("bowMode");
//  outbound.addByte(mode);
//  outbound.streamPacket(&Serial1);
//  delayMicroseconds(waitS);
//}
//
//void sndBowOn(byte mode){
//  outbound.beginPacket("bowOn");
//  outbound.addByte(mode);
//  outbound.streamPacket(&Serial1);
//  delayMicroseconds(waitS);
//}
//
//void sndEnv1(byte para, float val){
//  outbound.beginPacket("env1");
//  outbound.addByte(para);
//  outbound.addFloat(val);
//  outbound.streamPacket(&Serial1);
//}
//
//void sndEnv2(byte para, float val){
//  outbound.beginPacket("env2");
//  outbound.addByte(para);
//  outbound.addFloat(val);
//  outbound.streamPacket(&Serial1);
//}
//
//void sndFilter(byte para, float val){
//  outbound.beginPacket("fil");
//  outbound.addByte(para);
//  outbound.addFloat(val);
//  outbound.streamPacket(&Serial1); 
//}
//
//void sndFX(byte para, float val){
//  outbound.beginPacket("fx");
//  outbound.addByte(para);
//  outbound.addFloat(val);
//  outbound.streamPacket(&Serial1);
//}
//
//void sndLfo1(byte para, float val){
//  outbound.beginPacket("lfo1");
//  outbound.addByte(para);
//  outbound.addFloat(val);
//  outbound.streamPacket(&Serial1);
//}
//
//void sndVol(float val){
//  outbound.beginPacket("vol");
//  outbound.addFloat(val);
//  outbound.streamPacket(&Serial1);
//}
//
//void sndClock(byte type){
//  if(type==0)outbound.beginPacket("c");
//  if(type==1)outbound.beginPacket("start");
//  if(type==2)outbound.beginPacket("stop");
//  outbound.streamPacket(&Serial1);
//}
//
//void sndGetMidi(){
//  outbound.beginPacket("g");
//  outbound.streamPacket(&Serial1);
//}
//
//void sndCC(byte para,byte val){
//  outbound.beginPacket("cc");
//  outbound.addByte(para);
//  outbound.addByte(val);
//  outbound.streamPacket(&Serial1);
//}
//void sndTrnsp(byte para,byte val){
//  outbound.beginPacket("trnsp");
//  outbound.addByte(para);
//  outbound.addByte(val);
//  outbound.streamPacket(&Serial1);
//}
