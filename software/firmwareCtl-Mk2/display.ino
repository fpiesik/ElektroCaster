void updDisplay(){
  if (millis()-disp_frameTimer > disp_frameInt){
    disp_Clr();
    switch (opMode){
      case 0:
        strSetup_updDisp();
        break;
      case 1:
        scls_updDisp();
        break;
      case 2:
        strArp_updDisp();
        break;
      case 3:
        drmSq_updDisp();
        break;
    }
    disp_Buf();
    disp_frameTimer=millis();
  }
}

//display functions
void disp_Str(byte x, byte y, char* str) {
  msg_disp.beginPacket("str");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addString(str);
  msg_disp.streamPacket(&Serial4);
}

void disp_Int(byte x, byte y, int val) {
  msg_disp.beginPacket("int");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addInt(val);
  msg_disp.streamPacket(&Serial4);
}

void disp_Lng(byte x, byte y, long val) {
  msg_disp.beginPacket("lng");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addLong(val);
  msg_disp.streamPacket(&Serial4);
}

void disp_Frm(byte xp, byte yp, byte xs, byte ys) {
  msg_disp.beginPacket("frm");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.streamPacket(&Serial4);
}

void disp_Box(byte xp, byte yp, byte xs, byte ys) {
  msg_disp.beginPacket("box");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.streamPacket(&Serial4);
}

void disp_RFrm(byte xp, byte yp, byte xs, byte ys, byte r) {
  msg_disp.beginPacket("rfr");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_RBox(byte xp, byte yp, byte xs, byte ys, byte r) {
  msg_disp.beginPacket("rbx");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_Line(byte x0, byte y0, byte x1, byte y1) {
  msg_disp.beginPacket("lin");
  msg_disp.addByte(x0);
  msg_disp.addByte(y0);
  msg_disp.addByte(x1);
  msg_disp.addByte(y1);
  msg_disp.streamPacket(&Serial4);
}

void disp_Circle(byte x, byte y, byte r) {
  msg_disp.beginPacket("cir");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_Disk(byte x, byte y, byte r) {
  msg_disp.beginPacket("dis");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_Pix(byte x, byte y) {
  msg_disp.beginPacket("pix");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.streamPacket(&Serial4);
}

void disp_Color(byte c) {
  msg_disp.beginPacket("col");
  msg_disp.addByte(c);
  msg_disp.streamPacket(&Serial4);
}

void disp_Clr() {
  msg_disp.beginPacket("clr");
  msg_disp.streamPacket(&Serial4);
}

void disp_Buf() {
  msg_disp.beginPacket("buf");
  msg_disp.streamPacket(&Serial4);
}

void disp_debug(){
  disp_Color(1);
  disp_Line(5,5,100,20);
  disp_Circle(50, 40, 20);
  disp_Disk(50, 20, 20);
  disp_Box(100,10,10,20);
  disp_RBox(70,30,20,30, 7);
  disp_RFrm(70,30,20,30, 7);
  disp_Pix(60,10);
}
