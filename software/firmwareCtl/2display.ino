//#define BUFFER_LENGTH 64
char receivedString[BUFFER_LENGTH];

void toDisp() {
  if (millis()-dispTimer > dispIntvl){
    dispClr();
    
    //dispColor(1);
    //dispLine(5,5,100,20);
    //dispCircle(50, 40, 20);
    //dispDisk(50, 20, 20);
    //dispBox(100,10,10,20);
    //dispRBox(70,30,20,30, 7);
  //  dispRFrm(70,30,20,30, 7);
  //  dispPix(60,10);

  dispStr(75, 8, "bpm:");
  dispInt(103, 8, bpm);
  
  switch (dispEncFnc){
    case 0:
      dispRFrm(0, 23, 21, 19, 3);
      break;
    case 1:
      dispRFrm(21, 23, 90, 19, 3);
      break;
    case 2:
      dispRFrm(111, 23, 17, 19, 3);
      break;
  }
      
   // dispRFrm(0, 0, 128, 22, 5);
  //  dispFrm(0, 21, 128, 22);
  //  dispFrm(0, 42, 128, 22);
    dispStr(3, 55, toneNm[tuning[5] % 12]);
    dispStr(24, 55, toneNm[tuning[4] % 12]);
    dispStr(45, 55, toneNm[tuning[3] % 12]);
    dispStr(66, 55, toneNm[tuning[2] % 12]);
    dispStr(87, 55, toneNm[tuning[1] % 12]);
    dispStr(108, 55, toneNm[tuning[0] % 12]);
  
    //dispStr(3, 34, "Rt:");
    dispStr(3, 34, toneNm[rootNote % 12]);
    dispStr(24, 34, sclNm[scaleSel]);
    dispInt(115, 34, scaleStp+1);
    //dispStr(20,40,"helleo");
    //dispInt(100, 40, 6);
    dispBuf();
    
    dispTimer=millis();
  }
}

void dispStr(byte x, byte y, char* str) {
  outbound.beginPacket("str");
  outbound.addByte(x);
  outbound.addByte(y);
  outbound.addString(str);
  outbound.streamPacket(&Serial4);
}

void dispInt(byte x, byte y, int val) {
  outbound.beginPacket("int");
  outbound.addByte(x);
  outbound.addByte(y);
  outbound.addInt(val);
  outbound.streamPacket(&Serial4);
}

void dispLng(byte x, byte y, long val) {
  outbound.beginPacket("lng");
  outbound.addByte(x);
  outbound.addByte(y);
  outbound.addLong(val);
  outbound.streamPacket(&Serial4);
}

void dispFrm(byte xp, byte yp, byte xs, byte ys) {
  outbound.beginPacket("frm");
  outbound.addByte(xp);
  outbound.addByte(yp);
  outbound.addByte(xs);
  outbound.addByte(ys);
  outbound.streamPacket(&Serial4);
}

void dispBox(byte xp, byte yp, byte xs, byte ys) {
  outbound.beginPacket("box");
  outbound.addByte(xp);
  outbound.addByte(yp);
  outbound.addByte(xs);
  outbound.addByte(ys);
  outbound.streamPacket(&Serial4);
}

void dispRFrm(byte xp, byte yp, byte xs, byte ys, byte r) {
  outbound.beginPacket("rfr");
  outbound.addByte(xp);
  outbound.addByte(yp);
  outbound.addByte(xs);
  outbound.addByte(ys);
  outbound.addByte(r);
  outbound.streamPacket(&Serial4);
}

void dispRBox(byte xp, byte yp, byte xs, byte ys, byte r) {
  outbound.beginPacket("rbx");
  outbound.addByte(xp);
  outbound.addByte(yp);
  outbound.addByte(xs);
  outbound.addByte(ys);
  outbound.addByte(r);
  outbound.streamPacket(&Serial4);
}

void dispLine(byte x0, byte y0, byte x1, byte y1) {
  outbound.beginPacket("lin");
  outbound.addByte(x0);
  outbound.addByte(y0);
  outbound.addByte(x1);
  outbound.addByte(y1);
  outbound.streamPacket(&Serial4);
}

void dispCircle(byte x, byte y, byte r) {
  outbound.beginPacket("cir");
  outbound.addByte(x);
  outbound.addByte(y);
  outbound.addByte(r);
  outbound.streamPacket(&Serial4);
}

void dispDisk(byte x, byte y, byte r) {
  outbound.beginPacket("dis");
  outbound.addByte(x);
  outbound.addByte(y);
  outbound.addByte(r);
  outbound.streamPacket(&Serial4);
}

void dispPix(byte x, byte y) {
  outbound.beginPacket("pix");
  outbound.addByte(x);
  outbound.addByte(y);
  outbound.streamPacket(&Serial4);
}

void dispColor(byte c) {
  outbound.beginPacket("col");
  outbound.addByte(c);
  outbound.streamPacket(&Serial4);
}

void dispClr() {
  outbound.beginPacket("clr");
  outbound.streamPacket(&Serial4);
}

void dispBuf() {
  outbound.beginPacket("buf");
  outbound.streamPacket(&Serial4);
}
