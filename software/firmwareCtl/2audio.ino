#include "../shared/ProtocolAudio.h"

void sndLfo1(byte para, float val){
  Serial1.write(AUDIO_CMD_LFO_1);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndStrGain(int str, byte val) {
  Serial1.write(AUDIO_CMD_STR_GAIN);
  //delayMicroseconds(waitS);
  Serial1.write(str);
  //delayMicroseconds(waitS);
  Serial1.write(val*2); //string gain an the audio server has a max value of 99
  //delayMicroseconds(waitS);
//  msgOut_audio.beginPacket("strG");
//  msgOut_audio.addByte(str);
//  msgOut_audio.addInt(val);
//  msgOut_audio.streamPacket(&Serial1);
}

void sndVol(float val){
  Serial1.write(AUDIO_CMD_VOLUME);
  Serial1.write(byte(val*125)); //lowered to avoid clipping
}

void sndFilter(byte para, float val){
  Serial1.write(AUDIO_CMD_FILTER);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndEnv2(byte para, float val){
  Serial1.write(AUDIO_CMD_ENV_2);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndEnv1(byte para, float val){
  Serial1.write(AUDIO_CMD_ENV_1);
  //delayMicroseconds(waitS);
  Serial1.write(para);
  //delayMicroseconds(waitS);
  Serial1.write(byte(val*199));
}

void sndBowMode(byte mode){
  Serial1.write(AUDIO_CMD_BOW_MODE);
  Serial1.write(mode);
}

void sndBowOn(byte mode){
  Serial1.write(AUDIO_CMD_BOW_ON);
  Serial1.write(mode);
  //bowOn=mode;
}

void sndTrigEnv(byte str, float vel){
  Serial1.write(AUDIO_CMD_TRIG_ENV);
  //delayMicroseconds(waitS);
  Serial1.write(str);
  //delayMicroseconds(waitS);
  Serial1.write(byte(vel*199.0));
  //delayMicroseconds(waitS);
  
//  Serial.print("strnote:");
//  Serial.print(str);
//  Serial.print(" ");
//  Serial.println(str);
}

void sndStrPrs(byte str, byte pitch, byte state){
  Serial1.write(AUDIO_CMD_STR_FRET);
  //delayMicroseconds(waitS);
  Serial1.write(str);
  //delayMicroseconds(waitS);
  Serial1.write(pitch);
  Serial1.write(state);
  //delayMicroseconds(waitS);
}
