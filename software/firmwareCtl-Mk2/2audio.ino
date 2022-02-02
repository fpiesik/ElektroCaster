void sndVol(float val){
  Serial1.write(211);
  Serial1.write(byte(val*199));
}

void sndFilter(byte para, float val){
  Serial1.write(209);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndEnv1(byte para, float val){
  Serial1.write(207);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndEnv2(byte para, float val){
  Serial1.write(208);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndLfo1(byte para, float val){
  Serial1.write(216);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndBowMode(byte mode){
  Serial1.write(205);
  Serial1.write(mode);
}

void sndBowOn(byte mode){
  Serial1.write(206);
  Serial1.write(mode);
  //bowOn=mode;
}

void sndTrigEnv(byte str, float vel){
  Serial1.write(200);
  //delayMicroseconds(waitS);
  Serial1.write(str);
  //delayMicroseconds(waitS);
  Serial1.write(byte(vel*199));
  //delayMicroseconds(waitS);
  
//  Serial.print("strnote:");
//  Serial.print(str);
//  Serial.print(" ");
//  Serial.println(str);
}

void sndStrPrs(byte str, byte fret){
  Serial1.write(201);
  //delayMicroseconds(waitS);
  Serial1.write(str);
  //delayMicroseconds(waitS);
  Serial1.write(fret);
  //delayMicroseconds(waitS);

  Serial.print("str:");
  Serial.print(str);
  Serial.print(" fret:");
  Serial.println(fret);
}
