void rcvHidA(byte idx, int value){
//  if(dbgLvl>0){
//    Serial.print("a:");
//    Serial.print(idx);
//    Serial.print(" ");
//    Serial.println(value);
//    }
    
  sndDisp(0,idx,value);
  hidAVal[idx]=value/200.0;

}

void rcvHidD(byte idx, int value){
//  if(dbgLvl>0){
//    Serial.print("d:");
//    Serial.print(idx);
//    Serial.print(" ");
//    Serial.println(value);
//  }
  
  sndDisp(1,idx,value);
  hidDVal[idx]=value;
}
void rcvHidR(byte idx, int value){
//  if(dbgLvl>0){
//    Serial.print("r:");
//    Serial.print(idx);
//    Serial.print(" ");
//    Serial.println(value);
//  }
  sndDisp(2,idx,value);
  hidRVal[idx]=value;
}
void rcvHidE(byte idx, long value){
//  if(dbgLvl>0){
//    Serial.print("e:");
//    Serial.print(idx);
//    Serial.print(" ");
//    Serial.println(value);
//  }
  hidEVal4[idx]=hidEVal4[idx]+value;
  hidEVal[idx]=hidEVal4[idx]/4;
  sndDisp(3,idx,hidEVal[idx]);
  
}

void sndDisp(byte kind, byte idx, long val){
//  outbound.beginPacket("disp");
//  outbound.addByte(kind);
//  outbound.addByte(idx);
//  outbound.addLong(val);
//  outbound.addString(val);
//  outbound.streamPacket(&Serial4);
}

void sndGetHid(){
//  outbound.beginPacket("g");
//  outbound.streamPacket(&Serial4);
}
