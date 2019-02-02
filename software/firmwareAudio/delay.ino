void chDelay(byte para, float val){
   if(para==0)delay0.length(val*DELAY_MAX_LEN);
   if(para==1)mixer3.gain(1,val);
   if(para==2)mixer4.gain(1,val);
  }
