byte hrmSq_opMode=3;

void hrmSq_plcNotes(byte s){
  if(opStrMode==hrmSq_opMode){
    if(strPrs[s]>0){
      drmSq_stp[s][strPrs[s]-1][0]=(drmSq_stp[s][strPrs[s]-1][0]+1)%2;
      if(shift == 1)drmSq_stp[s][strPrs[s]-1][0]=0;
    }
    //lastStrUsed[s]=strUsed[s];
  }
}
