void rcvHidA(byte idx, int value){
  hidAVal[idx]=value/200.0;
}

void rcvHidD(byte idx, int value){
  hidDVal[idx]=value;
}

void rcvHidR(byte idx, int value){
  hidRVal[idx]=value;
}

void rcvHidE(byte idx, long value){
  hidEVal4[idx]=hidEVal4[idx]+value;
  hidEVal[idx]=hidEVal4[idx]/4;
}
