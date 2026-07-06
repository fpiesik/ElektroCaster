namespace {
const byte DEBUG_LINE_MAX = 48;
char debugLine[DEBUG_LINE_MAX];
byte debugLinePos = 0;
bool debugEnabled = false;

bool debugTokenEquals(const char* value, const char* token){
  while(*value != '\0' && *token != '\0'){
    if(*value != *token)return false;
    value++;
    token++;
  }
  return *value == '\0' && *token == '\0';
}

bool debugTokenStartsWith(const char* value, const char* prefix){
  while(*prefix != '\0'){
    if(*value != *prefix)return false;
    value++;
    prefix++;
  }
  return true;
}

const char* songStatusName(SongStatus status){
  switch(status){
    case SONG_OK: return "OK";
    case SONG_DEFAULT_USED_MISSING: return "DEFAULT_USED_MISSING";
    case SONG_DEFAULT_USED_BAD_MAGIC: return "DEFAULT_USED_BAD_MAGIC";
    case SONG_DEFAULT_USED_BAD_VERSION: return "DEFAULT_USED_BAD_VERSION";
    case SONG_DEFAULT_USED_BAD_SIZE: return "DEFAULT_USED_BAD_SIZE";
    case SONG_DEFAULT_USED_BAD_CRC: return "DEFAULT_USED_BAD_CRC";
    case SONG_DEFAULT_USED_BAD_VALUE: return "DEFAULT_USED_BAD_VALUE";
    case SONG_SAVE_FAILED: return "SAVE_FAILED";
    case SONG_ERR_SLOT_RANGE: return "ERR_SLOT_RANGE";
    default: return "UNKNOWN";
  }
}

void debugPrintSongStatus(){
  Serial.print("song slot=");
  Serial.print(lastSongSlot);
  Serial.print(" name=");
  if(songValidSlot(lastSongSlot))Serial.print(genSq_SongNm[lastSongSlot]);
  else Serial.print("<invalid>");
  Serial.print(" status=");
  Serial.println(songStatusName(lastSongStatus));
}

void debugPrintSongFile(int slot){
  Serial.print("song ");
  Serial.print(slot);
  if(!songValidSlot(slot)){
    Serial.println(" invalid-slot");
    return;
  }

  char fileName[24];
  songFileName(slot, fileName, sizeof(fileName));
  Serial.print(" name=");
  Serial.print(genSq_SongNm[slot]);
  Serial.print(" file=");
  Serial.print(fileName);
  Serial.print(" exists=");
  Serial.print(SD.exists(fileName) ? "yes" : "no");
  Serial.print(" current=");
  Serial.println(slot == genSq_actSng ? "yes" : "no");
}

void debugPrintSongs(){
  for(int slot = 0; slot < genSq_nSngs; slot++){
    debugPrintSongFile(slot);
  }
  debugPrintSongStatus();
}

void debugPrintStatus(){
  Serial.println("ElektroCaster firmwareCtl debug");
  Serial.print("millis=");
  Serial.println(millis());
  Serial.print("bpm=");
  Serial.print(bpm);
  Serial.print(" opMode=");
  Serial.print(opMode);
  Serial.print(" dispEncMode=");
  Serial.print(dispEncMode);
  Serial.print(" fbrdMode=");
  Serial.println(fbrdMode);
  Serial.print("clock running=");
  Serial.print(clckOn ? "yes" : "no");
  Serial.print(" external=");
  Serial.println(extClk ? "yes" : "no");
  debugPrintSongStatus();
}

void debugPrintHelp(){
  Serial.println("USB debug commands:");
  Serial.println("  debug on      enable debug output");
  Serial.println("  debug off     disable debug output");
  Serial.println("  debug status  print whether debug output is enabled");
  Serial.println("  help, ?       show this help");
  Serial.println("  status        print runtime and last song status");
  Serial.println("  song          print last song status");
  Serial.println("  songs         list configured song slots and files");
  Serial.println("  song <slot>   print one song slot, e.g. song 0");
}

void debugHandleLine(char* line){
  while(*line == ' ' || *line == '\t')line++;
  if(*line == '\0')return;

  if(debugTokenEquals(line, "debug on")){
    debugEnabled = true;
    Serial.println("debug enabled");
    return;
  }
  if(debugTokenEquals(line, "debug off")){
    Serial.println("debug disabled");
    debugEnabled = false;
    return;
  }
  if(debugTokenEquals(line, "debug status")){
    Serial.print("debug=");
    Serial.println(debugEnabled ? "on" : "off");
    return;
  }
  if(debugTokenEquals(line, "?") || debugTokenEquals(line, "help")){
    debugPrintHelp();
    return;
  }
  if(!debugEnabled){
    Serial.println("debug disabled; send 'debug on' to enable output");
    return;
  }
  if(debugTokenEquals(line, "status")){
    debugPrintStatus();
    return;
  }
  if(debugTokenEquals(line, "song")){
    debugPrintSongStatus();
    return;
  }
  if(debugTokenEquals(line, "songs")){
    debugPrintSongs();
    return;
  }
  if(debugTokenStartsWith(line, "song ")){
    int slot = atoi(line + 5);
    debugPrintSongFile(slot);
    return;
  }

  Serial.print("unknown debug command: ");
  Serial.println(line);
  debugPrintHelp();
}
}

void debugSetup(){
  Serial.println("USB debug ready but disabled. Type 'debug on' and press enter.");
}

void debugPoll(){
  while(Serial.available() > 0){
    char chr = (char)Serial.read();
    if(chr == '\r')continue;
    if(chr == '\n'){
      debugLine[debugLinePos] = '\0';
      debugHandleLine(debugLine);
      debugLinePos = 0;
      continue;
    }
    if(debugLinePos < DEBUG_LINE_MAX - 1){
      debugLine[debugLinePos++] = chr;
    }else{
      debugLinePos = 0;
      Serial.println("debug command too long");
    }
  }
}
