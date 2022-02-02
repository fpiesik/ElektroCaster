void serialEvent1(){
  int incoming=-1;
  byte sbyte = Serial1.read();
  if (sbyte > 199 && sbyte <= 255) incoming = sbyte - 200;
    if (incoming == 6){
    byte a;
    byte b;
    byte c;
    while(Serial1.available() == 0);
    a=Serial1.read();
    while(Serial1.available() == 0);
    b=Serial1.read();
    while(Serial1.available() == 0);
    c=Serial1.read();
    strP[a]=b+c/100.0;
  }
    if (incoming == 7){
    byte a;
    byte b;
    while(Serial1.available() == 0);
    a=Serial1.read();
    while(Serial1.available() == 0);
    b=Serial1.read();
    strA[a]=b/100.0;
  }  
}

void serialEvent4(){   
   
  int incoming=-1;
  byte serbyte = Serial4.read();
  if (serbyte > 200 && serbyte < 255) incoming = serbyte - 201;
  //if (serbyte == 255)sndGetMidi();

  if(incoming>=0){
    while(Serial4.available() == 0);
    byte val=Serial4.read();
    if (incoming <19) rcvHidD(incoming,val),incoming=-1;
    if (incoming >=19 && incoming < 38) rcvHidA(incoming-19,val);
    if (incoming >=38 && incoming < 41) rcvHidR(incoming-38,val);
    if (incoming >=41 && incoming < 49) rcvHidE(incoming-41,val-100);
  }
}
