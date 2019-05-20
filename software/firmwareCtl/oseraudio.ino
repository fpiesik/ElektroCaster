void rcvNote(byte pitch, byte velocity){
  float vel=velocity/127.0;
  midiNotes[pitch]=vel;
  if(vel>0&&lghtHold==1){
    lastMidiTones[pitch%12]=1;
    lastMidiNotes[pitch]=1;  
  }

  }

void allNOff(){
  for(int i=0;i<128;i++){
    midiNotes[i]=0;
  }
}

float scale(float val,byte op, float scl){
  if(op==0)return(val*scl);
  else if(op==1)return(val*val*scl);
  else return(val);
}

void rcvCC(byte cc, byte value){
  float val=value/127.0;
  for(int n=0;n<7;n++){
    if(cc==ccTd[0]){
      for(int s=0;s<nStrings;s++){
        byte tdI=scale(val,0,sclTd);
        tmDv[s]=tmDvs[tdI];
      }
    }
    if(cc==ccTd[n]&&n!=0){
      byte tdI=scale(val,0,sclTd);
      tmDv[n-1]=tmDvs[tdI];
    }
  }
  
  for(int n=0;n<7;n++){
    if(cc==ccArpRpt[0]){
      for(int s=0;s<nStrings;s++){
        arpRpt[s]=1+scale(val,0,sclArpRpt);
      }
    }
    if(cc==ccArpRpt[n]&&n!=0){
      arpRpt[n-1]=1+scale(val,0,sclArpRpt);
    }
  }
  
  if(cc==ccArpMode){
    arpMode=scale(val,0,sclArpMode);
  }
  if(cc==ccOpMode){
    if(val<=0.5)opmode=0;
    if(val>0.5)opmode=2;
  }
  if(cc==ccArpClkMode){
    arpClkMode=scale(val,0,sclArpClkMode);
  }
  if(cc==ccLghtHold){
    if(val<=0.5)lghtHold=0;
    if(val>0.5)lghtHold=1;
  }
  if(cc==ccLghtHoldRst){
    if(val>0.5)rstLghtHold();
  }
  if(cc==ccNotesOrTones){
    if(val<=0.5)NotesOrTones=0;
    if(val>0.5)NotesOrTones=1;
  }
} 

void strNote(byte str, float vel){
  Serial1.write(200);
  delayMicroseconds(waitS);
  Serial1.write(str);
  delayMicroseconds(waitS);
  Serial1.write(byte(vel*199));
  delayMicroseconds(waitS);
  
//  Serial.print("strnote:");
//  Serial.print(str);
//  Serial.print(" ");
//  Serial.println(str);
}

void strFret(byte str, byte fret){
  Serial1.write(201);
  delayMicroseconds(waitS);
  Serial1.write(str);
  delayMicroseconds(waitS);
  Serial1.write(fret);
  delayMicroseconds(waitS);

//  Serial.print("str:");
//  Serial.print(str);
//  Serial.print(" fret:");
//  Serial.println(fret);
}

void sndMode(byte mode){
  Serial1.write(202);
  Serial1.write(mode);
}

void sndDispMode(byte mode){
  Serial1.write(203);
  Serial1.write(mode);
}

void sndKickMode(byte mode){
  Serial1.write(204);
  Serial1.write(mode);
}

void sndBowMode(byte mode){
  Serial1.write(205);
  Serial1.write(mode);
}

void sndBowOn(byte mode){
  Serial1.write(206);
  Serial1.write(mode);
}

void sndEnv1(byte para, float val){
  Serial1.write(207);
  delayMicroseconds(waitS);
  Serial1.write(para);
  delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndEnv2(byte para, float val){
  Serial1.write(208);
  delayMicroseconds(waitS);
  Serial1.write(para);
  delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndFilter(byte para, float val){
  Serial1.write(209);
  delayMicroseconds(waitS);
  Serial1.write(para);
  delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndVol(float val){
  Serial1.write(211);
  Serial1.write(byte(val*199));
}

void sndClock(byte type){
  if(type==0)Serial1.write(212);
  if(type==1)Serial1.write(213);
  if(type==2)Serial1.write(214);
}

void sndFX(byte para, float val){
  Serial1.write(215);
  delayMicroseconds(waitS);
  Serial1.write(para);
  delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndLfo1(byte para, float val){
  Serial1.write(216);
  delayMicroseconds(waitS);
  Serial1.write(para);
  delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndGetMidi(){
  Serial1.write(217);
}

void sndTrnsp(byte para,byte val){
  Serial1.write(218);
  delayMicroseconds(waitS);
  Serial1.write(para);
  delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}
