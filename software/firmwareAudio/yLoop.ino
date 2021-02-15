void serialEvent5(){
        static int incoming;
        byte sbyte = Serial5.read();
        //Serial.write(serbyte);
        if (sbyte > 199 && sbyte <= 255) incoming = sbyte - 200;

//        if (incoming == 12) usbMIDI.sendRealTime(usbMIDI.Clock);
//        if (incoming == 13) usbMIDI.sendRealTime(usbMIDI.Start);
//        if (incoming == 14) usbMIDI.sendRealTime(usbMIDI.Stop);
        //if (incoming == 17)sndNotes();

        if (incoming == 0){
          byte a;
          byte b;
          while(Serial5.available() == 0);
          a=Serial5.read();
          while(Serial5.available() == 0);
          b=Serial5.read();
          seqHit(a, b/199.0);
          incoming = -1;
        }
         
        if (incoming == 1){
          byte a;
          byte b;
          while(Serial5.available() == 0);
          a=Serial5.read();
          while(Serial5.available() == 0);
          b=Serial5.read();
          //strFret(a, b);
          strState[a]=b;
          incoming = -1;
        }

        if (incoming == 7){
          byte a;
          byte b;
          while(Serial5.available() == 0);
          a=Serial5.read();
          while(Serial5.available() == 0);
          b=Serial5.read();
          float sclVal=scale(b/199.0,1,sclEnvA[a]);
          chEnvA(a,sclVal);
          incoming = -1;
        }

        if (incoming == 8){
          byte a;
          byte b;
          while(Serial5.available() == 0);
          a=Serial5.read();
          while(Serial5.available() == 0);
          b=Serial5.read();
          float sclVal=scale(b/199.0,1,sclEnvF[a]);
          chEnvF(a,sclVal);
          incoming = -1;
        }

        if (incoming == 9){
          byte a;
          byte b;
          while(Serial5.available() == 0);
          a=Serial5.read();
          while(Serial5.available() == 0);
          b=Serial5.read();
          valFilter[a]=scale(b/199.0,1,sclFilter[a]);
          chFilter(a,valFilter[a]);
          incoming = -1;
        }

        if (incoming == 15){
          byte a;
          byte b;
          while(Serial5.available() == 0);
          a=Serial5.read();
          while(Serial5.available() == 0);
          b=Serial5.read();
          valFX[a]=scale(b/199.0,1,sclFX[a]);
          chFX(a,valFX[a]);
          incoming = -1;
        }

        if (incoming == 16){
           byte a;
           float b;
           while(Serial5.available() == 0);
           a=Serial5.read();
           while(Serial5.available() == 0);
           b=Serial5.read();         
           valLfo1[a]=scale(b/199.0,1,sclLfo1[a]);
           for(int i=0;i<nStrings;i++){
            chLfo1(a,valLfo1[a],i);
           }
          incoming = -1;
          }

        if (incoming == 18){
           byte a;
           byte b;
           while(Serial5.available() == 0);
           a=Serial5.read();
           while(Serial5.available() == 0);
           b=Serial5.read();         
          if(a==2)sndMidiCC(3,b/2);
          incoming = -1;
          }
        
        
        if (sbyte <= 199){
                
          if (incoming == 2) chOpMode(sbyte),incoming = -1;
          if (incoming == 3) chDispMode(sbyte),incoming = -1;
          if (incoming == 4) chKickMode(sbyte),incoming = -1;
          if (incoming == 5) chBowMode(sbyte),incoming = -1;
          if (incoming == 6) bowOn=(sbyte),incoming = -1;

          if (incoming == 11){
            float val=sbyte/199.0;
            val=val*val;
            ampOut.gain(val+0.0001);
            Serial.println("gain: ");
            Serial.println(val);
            incoming = -1;
          }      
}
}

void loop(){
  usbMIDI.read();

//
//if (nFreq[0]->available()) {
//        float note = nFreq[0]->read();
//        float prob = nFreq[0]->probability();
//        double mNote = base_pitch + (12.0 * log(note / base_frequency) / log(2));
//        //Serial.printf("freq1: %3.2f | Probability: %.2f\n", note, prob);
//        Serial.printf("mNote1: %3.2f | Probability: %.2f\n", mNote, prob);
//    }
//
//if (nFreq2.available()) {
//        float note = nFreq2.read();
//        float prob = nFreq2.probability();
//        double mNote = base_pitch + (12.0 * log(note / base_frequency) / log(2));
//        //Serial.printf("freq2: %3.2f | Probability: %.2f\n", note, prob);
//        Serial.printf("mNote2: %3.2f | Probability: %.2f\n", mNote, prob);
//    }

//---Do control functions-----
if (millis()-ctlTimer > ctlInt){ 
  for(int i=0;i<nStrings;i++){
    if(strState[i]!=lastStrState[i]){
      strFret(i,strState[i]);
      lastStrState[i]=strState[i];
    }
  }
  ctlTimer=millis();
}

if (millis()-nFrqTimer > nFrqInt){ 
static int i=0;
static int cntSmp;
static int nSmp=5;
    if (nFreq[i]->available()) {
        float note = nFreq[i]->read();
        float prob = nFreq[i]->probability();
        
        float mNote = base_pitch + (12.0 * log(note / base_frequency) / log(2));
        if(prob>0.95){
          strP[i]=mNote;
          //Serial.printf("freq1: %3.2f | Probability: %.2f\n", note, prob);    
          Serial.print(i+1);
          Serial.print(" ");
          Serial.printf("mNote: %3.2f | Probability: %.2f\n", mNote, prob);
        }               
    } 
    cntSmp++;
    if (cntSmp>=nSmp){
      nFreq[i]->stop();
      nFreq[(i+1)%nStrings]->resume();
      cntSmp=0;
      i++;
      i=i%nStrings;
    }
    //else{
//      nFreq[i]->stop();
//      nFreq[(i+1)%nStrings]->resume(); 
    //}
  sndStrP();
  nFrqTimer=millis();
}


if(bowOn!=lastBowOn){
for (int i=0; i<nStrings; i++) {
    if(bowOn==0)cAmps[i]->gain(0.0001);
    if(bowOn==1&&strState[i]>0)cAmps[i]->gain(50);
  }
  lastBowOn=bowOn;
}
   
if(0) {
  if(millis() - last_time >= 3000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");    
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");    
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    last_time = millis();
  }
}
}
