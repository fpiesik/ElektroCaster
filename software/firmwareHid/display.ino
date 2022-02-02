void u8g2_ascii_1() {
  char s[2] = " ";
  uint8_t x, y;
  u8g2.drawStr( 0, 0, "ASCII page 1");
  for( y = 0; y < 6; y++ ) {
    for( x = 0; x < 16; x++ ) {
      s[0] = y*16 + x + 32;
      u8g2.drawStr(x*7, y*10+10, s);
    }
  }
}

void drwColor(byte c){
  u8g2.setDrawColor(c);
}

void drwStr(byte x, byte y, char* str){
  u8g2.drawStr(x, y, str);
}

void drwInt(byte x, byte y, int val){
  u8g2.setCursor(x, y);
  u8g2.print(val);
}

void drwLng(byte x, byte y, long val){
  u8g2.setCursor(x, y);
  u8g2.print(val);
}

void drwFrm(byte xp, byte yp, byte xs, byte ys){
  u8g2.drawFrame(xp,yp,xs,ys);
}
void drwBox(byte xp, byte yp, byte xs, byte ys){
  u8g2.drawBox(xp,yp,xs,ys);
}

void drwRFrm(byte xp, byte yp, byte xs, byte ys, byte r){
  u8g2.drawRFrame(xp,yp,xs,ys,r);
}

void drwRBox(byte xp, byte yp, byte xs, byte ys, byte r){
  u8g2.drawRBox(xp,yp,xs,ys,r);
}

void drwLine(byte x0, byte y0, byte x1, byte y1){
  u8g2.drawLine(x0,y0,x1,y1);
}

void drwCircle(byte x, byte y, byte r){
  u8g2.drawCircle(x,y,r, U8G2_DRAW_ALL);
}

void drwDisc(byte x, byte y, byte r){
  u8g2.drawDisc(x, y, r, U8G2_DRAW_ALL);
}

void drwPix(byte x, byte y){
  u8g2.drawPixel(x,y);
}




void u8g2_prepare(void) {
  //u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosCenter();
  u8g2.setFontDirection(0);
  u8g2.setFlipMode(1);
}


void drwVal(byte kind,byte idx, float val, char* str) {
//  u8g2.clearBuffer();
//  u8g2.setCursor(10, 10);
//  u8g2.print(kind);
//  u8g2.print(" ");
//  u8g2.print(idx);
//  u8g2.print(" ");
//  u8g2.print(val);
//  u8g2.drawStr(0,40,str);
//  
//  u8g2.sendBuffer();
}


//void drwVal(byte kind,byte idx, float val, char str) {
//  u8g2.clearBuffer();
//  u8g2.setFont(u8g2_font_9x15_tn);
//  u8g2.setCursor(10, 10);
//  u8g2.print(kind);
//  u8g2.print(" ");
//  u8g2.print(idx);
//  u8g2.print(" ");
//  u8g2.print(val);
//  u8g2.drawStr(0,40,"trft");
//  
//  u8g2.sendBuffer();
//}
