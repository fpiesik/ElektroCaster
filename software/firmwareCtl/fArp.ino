void mkArp(byte aM){
  byte flip=0;
  byte orderSrc=0;
  byte mirror=0;

  if(arpMode==2||arpMode==3||arpMode==6||arpMode==7)orderSrc=1;
  if(arpMode==1||arpMode==3||arpMode==5||arpMode==7)flip=1;
  if(arpMode==4||arpMode==5||arpMode==6||arpMode==7)mirror=1;

  ersSteps(); //clear the sequence
  arpSize=0; //reset arp size

  //calculate size of the sequence
  for(int s=0;s<nStrings;s++){
    if(rowUsed[s]>0)arpSize=arpSize+arpRpt[s];
  }
  
  int arpIdx=0;

  //-----make arp sequence by string order
  if(orderSrc==0){
    for(int s=0;s<nStrings;s++){
      if(rowUsed[s]>0){
        for(int r=0;r<arpRpt[s];r++){
          arpSeq[arpIdx]=s;
          arpIdx++;
        }  
      }
    }
  }

  //---make arp sequence by string press order---
  if(orderSrc==1){
    int skip=0;
    for(int i=0;i<arpSize;i++){
      int skipped=0;
      arpSeq[i]=pOrder[pOrderIdx-i-skip];
      if(rowUsed[arpSeq[i]]==0)skipped=1;
      for(int r=1;r<=i;r++){
        if(arpSeq[i]==arpSeq[i-r])skipped=1;
      }
      if(skipped>0){  
        i--;
        skip++; 
      }
    }
  }
  //----flip sequence----
  if(flip==1){
    byte seqbuf[arpSize];
    for(int i=0;i<arpSize;i++){
      seqbuf[i]=arpSeq[arpSize-i-1];
    }
    for(int i=0;i<arpSize;i++){
      arpSeq[i]=seqbuf[i];
    }
  }

  if(mirror==1){
    for(int i=0;i<arpSize;i++){
      arpSeq[i+arpSize]=arpSeq[arpSize-i-1];
    }
    arpSize=arpSize*2;
  }
  
  //---copy arp sequence to sequencer----
  for(int i=0;i<arpSize;i++){
  stepState[arpSeq[i]][i+1]=1;  
  }
  //---set seq length----
  for(int s=0;s<nStrings;s++){
  seqNStp[s]=arpSize;  
  }
}

void ersSteps(){
  for(int s=0;s<nStrings;s++){
    for(int f=0;f<nLedFrets;f++){
      stepState[s][f]=0;    
    }
  } 
}
