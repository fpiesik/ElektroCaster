void rcvHidA(byte idx, int value){
  Serial.print("a:");
  Serial.print(idx);
  Serial.print(" ");
  Serial.println(value);
  sndDisp(0,idx,value);
  hidAVal[idx]=value/1023.0;
}

void rcvHidD(byte idx, int value){
  Serial.print("d:");
  Serial.print(idx);
  Serial.print(" ");
  Serial.println(value);
  sndDisp(1,idx,value);
  hidDVal[idx]=value;
}
void rcvHidR(byte idx, int value){
  Serial.print("r:");
  Serial.print(idx);
  Serial.print(" ");
  Serial.println(value);
  sndDisp(2,idx,value);
  hidRVal[idx]=value;
}
void rcvHidE(byte idx, long value){
  Serial.print("e:");
  Serial.print(idx);
  Serial.print(" ");
  Serial.println(value);
  sndDisp(3,idx,value);
  hidEVal[idx]=value;
}

void sndDisp(byte kind, byte idx, long val){
  outbound.beginPacket("disp");
  outbound.addByte(kind);
  outbound.addByte(idx);
  outbound.addLong(val);
  outbound.streamPacket(&Serial3);
}

void sndGetHid(){
  outbound.beginPacket("g");
  outbound.streamPacket(&Serial3);
}
