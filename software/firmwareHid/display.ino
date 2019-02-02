void draw(void) {
  u8g2_prepare();
  u8g2_ascii_1();

}

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

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void drwVal(byte kind,byte idx, float val) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_9x15_tn);
  u8g2.setCursor(10, 10);
  u8g2.print(kind);
  u8g2.print(" ");
  u8g2.print(idx);
  u8g2.print(" ");
  u8g2.print(val);
  u8g2.sendBuffer();
}
