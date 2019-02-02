void sndA(byte idx, int val){
  outbound.beginPacket("a");
  outbound.addByte(idx);
  outbound.addInt(val);
  outbound.streamPacket(&Serial6);
}

void sndD(byte idx, int val){
  outbound.beginPacket("d");
  outbound.addByte(idx);
  outbound.addByte(val);
  outbound.streamPacket(&Serial6);
}

void sndR(byte idx, int val){
  outbound.beginPacket("r");
  outbound.addByte(idx);
  outbound.addByte(val);
  outbound.streamPacket(&Serial6);
}

void sndE(byte idx, int val){
  outbound.beginPacket("e");
  outbound.addByte(idx);
  outbound.addLong(val/4);
  outbound.streamPacket(&Serial6);
}

void sndF(){
  outbound.beginPacket("f");
  outbound.streamPacket(&Serial6);
}

void sendAllNew(){
  for(int i=0;i<nDigital;i++){
    if(dVal[i]!=lastDVal[i]){
      sndD(i,dVal[i]);
      lastDVal[i]=dVal[i];
    }
  }
 
  for(int i=0;i<nAnalog;i++){
    if(aVal[i]>lastAVal[i]+2 || aVal[i]<lastAVal[i]-2){
      if(i!=19&&i!=20&&i!=21){
        sndA(i,aVal[i]);
        lastAVal[i]=aVal[i];
      }
    }
  }

  if(newEnc0 != pos0)sndE(0,newEnc0),pos0 = newEnc0;
  if(newEnc1 != pos1)sndE(1,newEnc1),pos1 = newEnc1;
  if(newEnc2 != pos2)sndE(2,newEnc2),pos2 = newEnc2;
  if(newEnc3 != pos3)sndE(3,newEnc3),pos3 = newEnc3;
  if(newEnc4 != pos4)sndE(4,newEnc4),pos4 = newEnc4;
  if(newEnc5 != pos5)sndE(5,newEnc5),pos5 = newEnc5;
  if(newEnc6 != pos6)sndE(6,newEnc6),pos6 = newEnc6;
  if(newEnc7 != pos7)sndE(7,newEnc7),pos7 = newEnc7;
  
  for(int i=0;i<3;i++){
    if(rsState[i]!=lastRsState[i]){
      lastRsState[i]=rsState[i];
      sndR(i,rsState[i]);
    }
  }

  sndF();
}
