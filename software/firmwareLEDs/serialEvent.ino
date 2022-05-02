void serialEvent(){
        static int incoming;
        byte sbyte = Serial.read();
        //Serial.write(serbyte);
        if (sbyte > 250 && sbyte <= 255) incoming = sbyte - 250;

//        if (incoming == 12) usbMIDI.sendRealTime(usbMIDI.Clock);
//        if (incoming == 13) usbMIDI.sendRealTime(usbMIDI.Start);
//        if (incoming == 14) usbMIDI.sendRealTime(usbMIDI.Stop);
        //if (incoming == 17)sndNotes();

        if (incoming == 5){
          //FastLED.show();
          pixels.show();
          pixels.clear();
          incoming = -1;
        }
         
        if (incoming == 4){
          byte a;
          byte b;
          byte c[3];
          while(Serial.available() == 0);
          a=Serial.read();
          while(Serial.available() == 0);
          b=Serial.read();
          while(Serial.available() == 0);
          c[0]=Serial.read();
          while(Serial.available() == 0);
          c[1]=Serial.read();
          while(Serial.available() == 0);
          c[2]=Serial.read();
          pixel(a,b,c);
          incoming = -1;
        }
}
