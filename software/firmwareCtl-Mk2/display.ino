void updDisplay(){
  if (millis()-disp_frameTimer > disp_frameInt){
    disp_Clr();
    if(fbrdMode==0){
      switch (opMode){
        case 0:
          strSetup_updDisp();
          break;
        case 1:
          strArp_updDisp();
          break;
        case 2:
          scls_updDisp();
          break;
        case 3:
          scls_updDisp();
          //genSq_updDisp();
          break;
        case 4:
          scls_updDisp();
          //genSq_updDisp();
          break;
        case 5:
          scls_updDisp();
          //genSq_updDisp();
          break;
  
      }
    }
    if(fbrdMode==1){
      switch (opMode){
        case 0:
          strSetup_updDisp();
          break;
        case 1:
          strArp_updDisp();
          break;
        case 2:
          scls_updDisp();
          break;
        case 3:
          genSq_updDisp();
          break;
        case 4:
          genSq_updDisp();
          break;
        case 5:
          genSq_updDisp();
          break;
  
      }
    }
    disp_Buf();
    disp_frameTimer=millis();
  }
}

//display functions
void disp_Str(int x, int y, const char* str) {
  msg_disp.beginPacket("str");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addString(str);
  msg_disp.streamPacket(&Serial4);
}

void disp_Int(int x, int y, int val) {
  msg_disp.beginPacket("int");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addInt(val);
  msg_disp.streamPacket(&Serial4);
}

void disp_Lng(int x, int y, long val) {
  msg_disp.beginPacket("lng");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addLong(val);
  msg_disp.streamPacket(&Serial4);
}

void disp_Frm(int xp, int yp, int xs, int ys) {
  msg_disp.beginPacket("frm");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.streamPacket(&Serial4);
}

void disp_Box(int xp, int yp, int xs, int ys) {
  msg_disp.beginPacket("box");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.streamPacket(&Serial4);
}

void disp_RFrm(int xp, int yp, int xs, int ys, int r) {
  msg_disp.beginPacket("rfr");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_RBox(int xp, int yp, int xs, int ys, int r) {
  msg_disp.beginPacket("rbx");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_Line(int x0, int y0, int x1, int y1) {
  msg_disp.beginPacket("lin");
  msg_disp.addByte(x0);
  msg_disp.addByte(y0);
  msg_disp.addByte(x1);
  msg_disp.addByte(y1);
  msg_disp.streamPacket(&Serial4);
}

void disp_Circle(int x, int y, int r) {
  msg_disp.beginPacket("cir");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_Disk(int x, int y, int r) {
  msg_disp.beginPacket("dis");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial4);
}

void disp_Pix(int x, int y) {
  msg_disp.beginPacket("pix");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.streamPacket(&Serial4);
}

void disp_Color(int c) {
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
