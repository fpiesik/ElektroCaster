
//void chEnv1P(OSCMessage &msg, int adressOffset){
//envP[str]=data;
//  for (int i=0; i<nStrings; i++) {
//    if(str==0)envs[i]->delay(data);
//    if(str==1)envs[i]->attack(data);
//    if(str==2)envs[i]->hold(data);
//    if(str==3)envs[i]->decay(data);
//    if(str==4)envs[i]->sustain(data);
//    if(str==5)envs[i]->release(data);
//  }
//}

void chEnv1(byte para, float val){
 //if (msg.isFloat(0)){
  
  Serial.print("envP ");
  Serial.print(para);
  Serial.print(" ");
  Serial.println(val);

envP[para]=val;
  
  for (int i=0; i<nStrings; i++) {
    if(para==0)envs[i]->delay(val);
    if(para==1)envs[i]->attack(val);
    if(para==2)envs[i]->hold(val);
    if(para==3)envs[i]->decay(val);
    if(para==4)envs[i]->sustain(val);
    if(para==5)envs[i]->release(val);
  }
}

void chEnv2(byte para, float val){
 //if (msg.isFloat(0)){
  
  Serial.print("envF ");
  Serial.print(para);
  Serial.print(" ");
  Serial.println(val);

envF[para]=val;
  
  for (int i=0; i<nStrings; i++) {
    if(para==0)envs2[i]->delay(val);
    if(para==1)envs2[i]->attack(val);
    if(para==2)envs2[i]->hold(val);
    if(para==3)envs2[i]->decay(val);
    if(para==4)envs2[i]->sustain(val);
    if(para==5)envs2[i]->release(val);
  }
}
