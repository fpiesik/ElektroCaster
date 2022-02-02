void updEep(){
  for(int s=0;s<nStrings;s++){
    EEPROM.update(s, tuning[s]);
  }
   EEPROM.update(nStrings, rootNote);
   EEPROM.update(nStrings + 1, scaleSel);
}

void readEep(){
  for(int s=0;s<nStrings;s++){
    tuning[s]=EEPROM.read(s);
  }
  rootNote=EEPROM.read(nStrings);
  scaleSel=EEPROM.read(nStrings + 1);
}
