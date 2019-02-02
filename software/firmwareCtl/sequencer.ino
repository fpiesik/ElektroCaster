float pixState[nStrings][nLedFrets][3];

void updateSteps(){
static byte lastRowUsed[6];
//readFretboard();
for(int s=0;s<nStrings; s++){
  if(lastRowUsed[s]!=rowUsed[s]&&rowUsed[s]>0)stepState[s][rowUsed[s]]=!stepState[s][rowUsed[s]];
  lastRowUsed[s]=rowUsed[s];
  }
}

void updNStp(){
static byte lastRowUsed[6];
//readFretboard();
for(int s=0;s<nStrings; s++){
  if(lastRowUsed[s]!=rowUsed[s]&&rowUsed[s]>0)seqNStp[s]=rowUsed[s];
  lastRowUsed[s]=rowUsed[s];
  }
}

void updSeqSpd(){
static byte lastRowUsed[6];
//readFretboard();
for(int s=0;s<nStrings; s++){
  if(lastRowUsed[s]!=rowUsed[s]&&rowUsed[s]>0)seqSpd[s]=rowUsed[s];
  lastRowUsed[s]=rowUsed[s];
  }
}

void eraseSteps(){
static byte lastRowUsed[6];
//readFretboard();
for(int s=0;s<nStrings; s++){
  if(lastRowUsed[s]!=rowUsed[s]&&rowUsed[s]>0)stepState[s][rowUsed[s]]=0;
  lastRowUsed[s]=rowUsed[s];
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
  pixels.setPixelColor(pixPos[s][f], pixels.Color(cComp(0,pixState[s][f][0]),cComp(1,pixState[s][f][1]),cComp(2,pixState[s][f][2])));
  //setPixel(pixPos[s][f], cComp(0,pixState[s][f][0]),cComp(1,pixState[s][f][1]),cComp(2,pixState[s][f][2]));
  }}
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
