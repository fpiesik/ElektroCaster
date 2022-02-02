void readFretboard() {
  long Millis=millis();
  for (int s=0; s < nStrings; s++) {
    if(Millis-lastFretRead[s]>fretReadInt){  
      if(shift != 1)lastStrUsed[s]=strUsed[s];
      strUsed[s]=!digitalRead(strSnsPins[s]);
      if (strUsed[s]!=lastStrUsed[s]){
        strFret(s, strUsed[s]);
        if((runSeq==0||bowOn==1) && strUsed[s]==1){
          if(kickSeq==1)kickCue[s]=1;
          if(runSeq==0)strNote(s,1); 
        }
        if(runSeq==0 && strUsed[s]==0){
        strNote(s,0); 
        }
        lastFretRead[s]=Millis;
      }
    }
//probably couses bug, needs investigation
//    if(lastStrUsed[s]==0&&strUsed[s]>0 && pOrder[pOrderIdx]!=s){
//      pOrderIdx++;
//      pOrder[pOrderIdx]=s;
//    } 
  
  }
}
