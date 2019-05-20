void mkSeq(byte aM){

  ersSteps(); //clear the sequence

  for(int s=0;s<nStrings;s++){
  seqNStp[s]=16;  
  }

    for(int s=0;s<nStrings;s++){
      if(strUsed[s]>0){
        for(int i=0;i<=seqNStp[s]/seqNDur[s];i++){    
          byte stp=1+seqNDur[s]*i;
          if (stp<=16)stepState[s][stp]=1;
        }   
      }
    }
  
  //---set seq length----

}
