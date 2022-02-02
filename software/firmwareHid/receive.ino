void serialEvent6() { 
  if (inbound.parseStream( &Serial6 )) {
    
    if (inbound.fullMatch("tuner")) {
//      char receivedString[BUFFER_LENGTH];
//      byte kind = inbound.nextByte();
//      byte idx = inbound.nextInt();
//      long val = inbound.nextLong();
//      //char str[]=inbound.nextString()
//      int stringLength = inbound.nextString(receivedString,BUFFER_LENGTH);
//
//      //char str=receivedString;
//      //drwVal(kind,idx,val,receivedString);
//      drwTuner(receivedString);
    }

    if (inbound.fullMatch("str")) {
      char receivedString[BUFFER_LENGTH];
      byte x = inbound.nextByte();
      byte y = inbound.nextByte();
      int stringLength = inbound.nextString(receivedString,BUFFER_LENGTH);
      drwStr(x, y, receivedString);
    }

    if (inbound.fullMatch("int")) {
      byte x = inbound.nextByte();
      byte y = inbound.nextByte();
      int val = inbound.nextInt();
      drwInt(x, y, val);
    }
    if (inbound.fullMatch("lng")) {
      byte x = inbound.nextByte();
      byte y = inbound.nextByte();
      int val = inbound.nextLong();
      drwLng(x, y, val);
    }

    if (inbound.fullMatch("frm")) {
      byte xp = inbound.nextByte();
      byte yp = inbound.nextByte();
      byte xs = inbound.nextByte();
      byte ys = inbound.nextByte();
      drwFrm(xp, yp, xs, ys);
    }

    if (inbound.fullMatch("box")) {
      byte xp = inbound.nextByte();
      byte yp = inbound.nextByte();
      byte xs = inbound.nextByte();
      byte ys = inbound.nextByte();
      drwBox(xp, yp, xs, ys);
    }

    if (inbound.fullMatch("rfr")) {
      byte xp = inbound.nextByte();
      byte yp = inbound.nextByte();
      byte xs = inbound.nextByte();
      byte ys = inbound.nextByte();
      byte r = inbound.nextByte();
      drwRFrm(xp, yp, xs, ys, r);
    }

    if (inbound.fullMatch("rbx")) {
      byte xp = inbound.nextByte();
      byte yp = inbound.nextByte();
      byte xs = inbound.nextByte();
      byte ys = inbound.nextByte();
      byte r = inbound.nextByte();
      drwRBox(xp, yp, xs, ys, r);
    }

    if (inbound.fullMatch("cir")) {
      byte x = inbound.nextByte();
      byte y = inbound.nextByte();
      byte r = inbound.nextByte();
      drwCircle(x, y, r);
    }

    if (inbound.fullMatch("dis")) {
      byte x = inbound.nextByte();
      byte y = inbound.nextByte();
      byte r = inbound.nextByte();
      drwDisc(x, y, r);
    }

    if (inbound.fullMatch("pix")) {
      byte x = inbound.nextByte();
      byte y = inbound.nextByte();
      drwPix(x, y);
    }
    
    if (inbound.fullMatch("col")) {
      byte c = inbound.nextByte();
      drwColor(c);
    }
    
    if (inbound.fullMatch("lin")) {
      byte x0 = inbound.nextByte();
      byte y0 = inbound.nextByte();
      byte x1 = inbound.nextByte();
      byte y1 = inbound.nextByte();
      drwLine(x0, y0, x1, y1);
    }
    
    if (inbound.fullMatch("clr")) {
      u8g2.clearBuffer();
    }
    if (inbound.fullMatch("buf")) {
      u8g2.sendBuffer();
    }
  }
}
