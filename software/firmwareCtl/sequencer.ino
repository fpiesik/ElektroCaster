float pixState[nStrings][nLedFrets][3];

void updateSteps(){
//static byte lastStrUsed[6];
for(int s=0;s<nStrings; s++){
  if(lastStrUsed[s]!=strUsed[s]&&strUsed[s]>0)stepState[s][strUsed[s]]=!stepState[s][strUsed[s]];
  //lastStrUsed[s]=strUsed[s];
  }
}

void updNStp(){
//static byte lastStrUsed[6];
//readFretboard();
for(int s=0;s<nStrings; s++){
  if(lastStrUsed[s]!=strUsed[s]&&strUsed[s]>0)seqNStp[s]=strUsed[s];
  //lastStrUsed[s]=strUsed[s];
  }
}

void updSeqSpd(){
//static byte lastStrUsed[6];
//readFretboard();
for(int s=0;s<nStrings; s++){
  if(lastStrUsed[s]!=strUsed[s]&&strUsed[s]>0)seqSpd[s]=strUsed[s];
  //lastStrUsed[s]=strUsed[s];
  }
}

void eraseSteps(){
//static byte lastStrUsed[6];
//readFretboard();
for(int s=0;s<nStrings; s++){
  if(lastStrUsed[s]!=strUsed[s]&&strUsed[s]>0)stepState[s][strUsed[s]]=0;
  //lastStrUsed[s]=strUsed[s];
  }
}

void ersSteps(){
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<nLedFrets;f++){
      stepState[s][f]=0;    
    }
  } 
}

void seqGrid(){
  float colorA[3]={0.4,0,0.0};
  float colorB[3]={0.05,0,0.0};
  for(int s=0;s<nStrings;s++){
  for(int f=0;f<seqNStp[s];f++){
  if(f%4==0){
    pixState[s][f+1][0]=pixState[s][f+1][0]+colorA[0];
    pixState[s][f+1][1]=pixState[s][f+1][1]+colorA[1];
    pixState[s][f+1][2]=pixState[s][f+1][2]+colorA[2];
  }
   if(f%4!=0){
    pixState[s][f+1][0]=pixState[s][f+1][0]+colorB[0];
    pixState[s][f+1][1]=pixState[s][f+1][1]+colorB[1];
    pixState[s][f+1][2]=pixState[s][f+1][2]+colorB[2];
  }
  }}
}
void seqMoveIdx(){
  float color[3]={0.5,0.5,0};
  for(int s=0;s<nStrings;s++){
  for(int f=1;f<nLedFrets;f++){
  if(f==seqIdx[s]){
    pixState[s][f][0]=pixState[s][f][0]+color[0];
    pixState[s][f][1]=pixState[s][f][1]+color[1];
    pixState[s][f][2]=pixState[s][f][2]+color[2];
  }
   }}
  
  //lastSeqIdx=seqIdx;
}
void steps(){
  float color[3]={0,0.5,0.5};
  for(int s=0;s<nStrings;s++){ 
    if(arpMute[s]==0)color[0]=0,color[1]=0.5,color[2]=0.5;
    if(arpMute[s]==1)color[0]=0.1,color[1]=0.1,color[2]=0.5;
   
  for(int f=0;f<nLedFrets;f++){ 
  if(stepState[s][f]==1){
    pixState[s][f][0]=pixState[s][f][0]+color[0];
    pixState[s][f][1]=pixState[s][f][1]+color[1];
    pixState[s][f][2]=pixState[s][f][2]+color[2];
  }
  }}
}
void seqMerge(){
  for(int s=0;s<nStrings;s++){ 
  for(int f=0;f<nLedFrets;f++){ 
  if(pixState[s][f][0]>1)pixState[s][f][0]=1; 
  if(pixState[s][f][1]>1)pixState[s][f][1]=1; 
  if(pixState[s][f][2]>1)pixState[s][f][2]=1;
  float c[3];
  c[0]=pixState[s][f][0];
  c[1]=pixState[s][f][1];
  c[2]=pixState[s][f][2];
  pixel(s,f,c); 
  }
  }
}
void mkFrame(){
  for(int s=0;s<nStrings;s++){ 
  for(int f=0;f<nLedFrets;f++){  
  pixState[s][f][0]=0;
  pixState[s][f][1]=0;
  pixState[s][f][2]=0;}}

  seqGrid();
  steps();
  seqMoveIdx();
  seqMerge();
  //pixels.show();
}
