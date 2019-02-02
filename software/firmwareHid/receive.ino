void serialEvent6() { 
  if (inbound.parseStream( &Serial6 )) {
    if (inbound.fullMatch("disp")) {
      byte kind = inbound.nextByte();
      byte idx = inbound.nextInt();
      long val = inbound.nextLong();
      
      drwVal(kind,idx,val);
    }
    if (inbound.fullMatch("g")) {
      sendAllNew();
    }
  }
}
