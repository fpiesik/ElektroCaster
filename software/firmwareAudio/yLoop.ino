#include "../shared/ProtocolAudio.h"

void serialEvent1(){
        static int incoming;
        byte sbyte = Serial1.read();
        //Serial.write(serbyte);
        if (sbyte >= AUDIO_CMD_BASE && sbyte <= AUDIO_CMD_MAX) incoming = audioIncoming(sbyte);

//        if (incoming == 12) usbMIDI.sendRealTime(usbMIDI.Clock);
//        if (incoming == 13) usbMIDI.sendRealTime(usbMIDI.Start);
//        if (incoming == 14) usbMIDI.sendRealTime(usbMIDI.Stop);
        //if (incoming == audioIncoming(AUDIO_CMD_BPM))sndNotes();

        if (incoming == audioIncoming(AUDIO_CMD_TRIG_ENV)){
          byte a;
          float b;
          while(Serial1.available() == 0);
          a=Serial1.read();
          while(Serial1.available() == 0);
          b=Serial1.read();
          //Serial.println(b);
          trigEnv(a, b/199.0);
          
          incoming = -1;
        }
         
        if (incoming == audioIncoming(AUDIO_CMD_STR_FRET)){
          byte a;
          byte b;
          byte c;
          while(Serial1.available() == 0);
          a=Serial1.read();
          while(Serial1.available() == 0);
          b=Serial1.read();
          while(Serial1.available() == 0);
          c=Serial1.read();
          strFret(a, b, c);
          strState[a]=c;
          
          incoming = -1;
        }

        if (incoming == audioIncoming(AUDIO_CMD_ENV_1)){
          byte a;
          byte b;
          while(Serial1.available() == 0);
          a=Serial1.read();
          while(Serial1.available() == 0);
          b=Serial1.read();
          float sclVal=scale(b/199.0,1,sclEnvA[a]);
          chEnvA(a,sclVal);
          envPA[a]=sclVal;
          incoming = -1;
        }

        if (incoming == audioIncoming(AUDIO_CMD_ENV_2)){
          byte a;
          byte b;
          while(Serial1.available() == 0);
          a=Serial1.read();
          while(Serial1.available() == 0);
          b=Serial1.read();
          float sclVal=scale(b/199.0,1,sclEnvF[a]);
          chEnvF(a,sclVal);
          envPF[a]=sclVal;
          incoming = -1;
        }

        if (incoming == audioIncoming(AUDIO_CMD_FILTER)){
          byte a;
          byte b;
          while(Serial1.available() == 0);
          a=Serial1.read();
          while(Serial1.available() == 0);
          b=Serial1.read();
          valFilter[a]=scale(b/199.0,1,sclFilter[a]);
          chFilter(a,valFilter[a]);
          incoming = -1;
        }

        if (incoming == audioIncoming(AUDIO_CMD_STR_GAIN)){
           byte a;
           byte b;
           float val;
           while(Serial1.available() == 0);
           a=Serial1.read();
           while(Serial1.available() == 0);
           b=Serial1.read();  
           val=b/100.0;      
           chngStrOutGain(a, val);
            Serial.print(a);
            Serial.print(": ");
            Serial.println(val);
           incoming = -1;
          }

        if (incoming == audioIncoming(AUDIO_CMD_FX)){
          byte a;
          byte b;
          while(Serial1.available() == 0);
          a=Serial1.read();
          while(Serial1.available() == 0);
          b=Serial1.read();
          valFX[a]=scale(b/199.0,1,sclFX[a]);
          chFX(a,valFX[a]);
          incoming = -1;
        }

        if (incoming == audioIncoming(AUDIO_CMD_LFO_1)){
           byte a;
           float b;
           while(Serial1.available() == 0);
           a=Serial1.read();
           while(Serial1.available() == 0);
           b=Serial1.read();         
           valLfo1[a]=scale(b/199.0,1,sclLfo1[a]);
           for(int i=0;i<nStrings;i++){
            chLfo1(a,valLfo1[a],i);
           }
          incoming = -1;
          }

        if (incoming == audioIncoming(AUDIO_CMD_MIDI_CC)){
           byte a;
           byte b;
           while(Serial1.available() == 0);
           a=Serial1.read();
           while(Serial1.available() == 0);
           b=Serial1.read();         
          if(a==2)sndMidiCC(3,b/2);
          incoming = -1;
          }

    
        
        if (sbyte <= 199){
                
          if (incoming == audioIncoming(AUDIO_CMD_OP_MODE)) chOpMode(sbyte),incoming = -1;
          if (incoming == audioIncoming(AUDIO_CMD_DISP_MODE)) chDispMode(sbyte),incoming = -1;
          if (incoming == audioIncoming(AUDIO_CMD_KICK_MODE)) chKickMode(sbyte),incoming = -1;
          if (incoming == audioIncoming(AUDIO_CMD_BOW_MODE)) chBowMode(sbyte),incoming = -1;
          if (incoming == audioIncoming(AUDIO_CMD_BOW_ON)) bowOn=(sbyte),incoming = -1;

        if (incoming == audioIncoming(AUDIO_CMD_VOLUME)){
          float val=sbyte/199.0;
          val=val*val*2;
          ampOut.gain(val+0.0001);
          //Serial.println("gain: ");
          //Serial.println(val);
          incoming = -1;
        }
        if (incoming == audioIncoming(AUDIO_CMD_BPM)){
          float val=sbyte;
          bpm=val;
          //Serial.println("bpm: ");
          //Serial.println(val);
          incoming = -1;
        }   
        }
}

void loop(){
  //usbMIDI.read();

//---Do control functions-----
if (millis()-ctlTimer > ctlInt){ 
  for(int i=0;i<nStrings;i++){
    if(strState[i]!=lastStrState[i]){
      //strFret(i,strState[i]);
      lastStrState[i]=strState[i];
    }
  }
  ctlTimer=millis();
}

if (millis()-nFrqTimer > nFrqInt){ 
static int i=0;
static int cntSmp;
static int nSmp=5;
  for(int s=0;s<nStrings;s++){
    nFreq[s].resume();
    if (nFreq[s].available()) {
        float note = nFreq[s].read();
        float prob = nFreq[s].probability();
        
        float mNote = base_pitch + (12.0 * log(note / base_frequency) / log(2));
        if(prob>0.96){
          strP[s]=mNote;
          //chLfo1(0, mNote,s);
          //chCoilDel(s,note);
//          Serial.printf("freq1: %3.2f | Probability: %.2f\n", note, prob);    
//          Serial.print(i+1);
//          Serial.print(" ");
//          Serial.printf("mNote: %3.2f | Probability: %.2f\n", mNote, prob);
        }               
    
    }
    //nFreq[s].stop(); 
  }
//    cntSmp++;
//    if (cntSmp>=nSmp){
//      nFreq[i].stop();
//      nFreq[(i+1)%nStrings].resume();
//      cntSmp=0;
//      i++;
//      i=i%nStrings;
//    }

  sndStrP();
  
  nFrqTimer=millis();
}

if (millis()-peakTimer > peakInt){ 
  for(int s=0; s<nStrings; s++){
    if (strPeak[s].available()) {
      float vel = strPeak[s].read() * 5.0;
      //Serial.printf("freq1: %3.2f | Probability: %.2f\n", note, prob);
      if(vel>1.0)vel=1.0;  
      strA[s]=vel;  
//      Serial.print(s+1);
//      Serial.print(" ");
//      Serial.print("Vel: "); 
//      Serial.println(vel);          
    } 
  }
  sndStrA();
  peakTimer=millis();
}

if(bowOn!=lastBowOn){
  // for (int i=0; i<nStrings; i++) {
  //   if(bowOn==0)coilAmp[i].gain(0.000);
  //   if(bowOn==1 && strState[i]>0)coilAmp[i].gain(100);
  // }
  for (int str=0; str<nStrings; str++) {
    if(bowOn==1 && strState[str]>0)coilOsc[str].amplitude(1);
    if(bowOn==0)coilOsc[str].amplitude(0);
  }

  Serial.print("bowOn: ");
  Serial.println(bowOn);
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
