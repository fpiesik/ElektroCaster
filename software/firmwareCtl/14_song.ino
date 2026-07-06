const char songMagic[4] = {'E', 'C', 'S', 'G'};
const uint16_t songFormatVersion = 1;
const uint16_t songHeaderSize = 32;
const uint16_t songChunkHeaderSize = 10;
const uint16_t songHeaderUsedSize = 26;
const uint16_t songHeaderReservedSize = songHeaderSize - songHeaderUsedSize;
const uint32_t songCrcSeed = 0xFFFFFFFFUL;
const uint32_t songCrcFinalXor = 0xFFFFFFFFUL;

const char songMetaChunkId[4] = {'M', 'E', 'T', 'A'};
const char songSeqChunkId[4] = {'S', 'E', 'Q', ' '};
const char songMixChunkId[4] = {'M', 'I', 'X', ' '};
const char songEndChunkId[4] = {'E', 'N', 'D', ' '};

struct SongFileHeader {
  uint16_t version;
  uint16_t headerSize;
  uint32_t fileSize;
  uint32_t payloadSize;
  uint32_t crc32;
  uint8_t nInst;
  uint8_t nStr;
  uint8_t nPttn;
  uint8_t maxSteps;
  uint8_t nStrEncFnc;
  uint8_t nStrPrsFnc;
};

struct SongChunkInfo {
  uint32_t payloadStart;
  uint32_t payloadLength;
};

struct SongData {
  uint8_t sclQ[genSq_nInst][nStrings];
  uint8_t chn[genSq_nInst][genSq_nPttn][nStrings][genSq_nStrEncFnc];
  uint8_t stpOnOff[genSq_nInst][genSq_nPttn][nStrings][genSq_maxSteps];
  uint8_t stp[genSq_nInst][genSq_nPttn][nStrings][genSq_maxSteps][genSq_nStrPrsFnc];
  uint8_t rootNote;
  uint8_t sclSel;
  uint8_t sclStp;
  uint8_t sclClr;
  uint8_t songBpm;
  uint8_t fledSrc;
  uint8_t songTuning[nStrings];
  uint8_t songStrGain[nStrings];
};

enum SongStatus {
  SONG_OK,
  SONG_DEFAULT_USED_MISSING,
  SONG_DEFAULT_USED_BAD_MAGIC,
  SONG_DEFAULT_USED_BAD_VERSION,
  SONG_DEFAULT_USED_BAD_SIZE,
  SONG_DEFAULT_USED_BAD_CRC,
  SONG_DEFAULT_USED_BAD_VALUE,
  SONG_SAVE_FAILED,
  SONG_ERR_SLOT_RANGE
};

SongStatus lastSongStatus = SONG_OK;
int lastSongSlot = 0;
static SongData songBuffer;
static SongData songVerifyBuffer;

uint32_t songCrcUpdate(uint32_t crc, uint8_t data){
  crc ^= data;
  for(uint8_t bit = 0; bit < 8; bit++){
    if(crc & 1)crc = (crc >> 1) ^ 0xEDB88320UL;
    else crc >>= 1;
  }
  return crc;
}

bool songValidSlot(int sng){
  return sng >= 0 && sng < genSq_nSngs;
}

void songFileName(int sng, char* out, size_t outLen){
  snprintf(out, outLen, "%s.ecs", genSq_SongNm[sng]);
}

void songTempFileName(int sng, char* out, size_t outLen){
  snprintf(out, outLen, "%s.tmp", genSq_SongNm[sng]);
}

bool songWriteByte(File& file, uint8_t value, uint32_t* crc){
  if(file.write(value) != 1)return false;
  if(crc != NULL)*crc = songCrcUpdate(*crc, value);
  return true;
}

bool songWriteU16(File& file, uint16_t value, uint32_t* crc){
  return songWriteByte(file, value & 0xFF, crc) && songWriteByte(file, (value >> 8) & 0xFF, crc);
}

bool songWriteU32(File& file, uint32_t value, uint32_t* crc){
  return songWriteByte(file, value & 0xFF, crc) &&
         songWriteByte(file, (value >> 8) & 0xFF, crc) &&
         songWriteByte(file, (value >> 16) & 0xFF, crc) &&
         songWriteByte(file, (value >> 24) & 0xFF, crc);
}

bool songWriteId(File& file, const char* id, uint32_t* crc){
  for(uint8_t i = 0; i < 4; i++){
    if(!songWriteByte(file, id[i], crc))return false;
  }
  return true;
}

int songReadByte(File& file, uint32_t* crc){
  int value = file.read();
  if(value < 0)return -1;
  if(crc != NULL)*crc = songCrcUpdate(*crc, (uint8_t)value);
  return value;
}

bool songReadU16(File& file, uint16_t* value, uint32_t* crc){
  int lo = songReadByte(file, crc);
  int hi = songReadByte(file, crc);
  if(lo < 0 || hi < 0)return false;
  *value = (uint16_t)lo | ((uint16_t)hi << 8);
  return true;
}

bool songReadU32(File& file, uint32_t* value, uint32_t* crc){
  int b0 = songReadByte(file, crc);
  int b1 = songReadByte(file, crc);
  int b2 = songReadByte(file, crc);
  int b3 = songReadByte(file, crc);
  if(b0 < 0 || b1 < 0 || b2 < 0 || b3 < 0)return false;
  *value = (uint32_t)b0 | ((uint32_t)b1 << 8) | ((uint32_t)b2 << 16) | ((uint32_t)b3 << 24);
  return true;
}

bool songReadId(File& file, char* id, uint32_t* crc){
  for(uint8_t i = 0; i < 4; i++){
    int value = songReadByte(file, crc);
    if(value < 0)return false;
    id[i] = (char)value;
  }
  return true;
}

bool songIdEquals(const char* a, const char* b){
  for(uint8_t i = 0; i < 4; i++){
    if(a[i] != b[i])return false;
  }
  return true;
}

bool songWriteHeader(File& file, uint32_t fileSize, uint32_t payloadSize, uint32_t crc){
  if(!songWriteId(file, songMagic, NULL))return false;
  if(!songWriteU16(file, songFormatVersion, NULL))return false;
  if(!songWriteU16(file, songHeaderSize, NULL))return false;
  if(!songWriteU32(file, fileSize, NULL))return false;
  if(!songWriteU32(file, payloadSize, NULL))return false;
  if(!songWriteU32(file, crc, NULL))return false;
  if(!songWriteByte(file, genSq_nInst, NULL))return false;
  if(!songWriteByte(file, nStrings, NULL))return false;
  if(!songWriteByte(file, genSq_nPttn, NULL))return false;
  if(!songWriteByte(file, genSq_maxSteps, NULL))return false;
  if(!songWriteByte(file, genSq_nStrEncFnc, NULL))return false;
  if(!songWriteByte(file, genSq_nStrPrsFnc, NULL))return false;
  for(uint8_t i = 0; i < songHeaderReservedSize; i++){
    if(!songWriteByte(file, 0, NULL))return false;
  }
  return true;
}

bool songReadHeader(File& file, struct SongFileHeader* header){
  char magic[4];
  if(!songReadId(file, magic, NULL))return false;
  if(!songIdEquals(magic, songMagic)){
    lastSongStatus = SONG_DEFAULT_USED_BAD_MAGIC;
    return false;
  }
  if(!songReadU16(file, &header->version, NULL))return false;
  if(!songReadU16(file, &header->headerSize, NULL))return false;
  if(!songReadU32(file, &header->fileSize, NULL))return false;
  if(!songReadU32(file, &header->payloadSize, NULL))return false;
  if(!songReadU32(file, &header->crc32, NULL))return false;
  int value = songReadByte(file, NULL); if(value < 0)return false; header->nInst = value;
  value = songReadByte(file, NULL); if(value < 0)return false; header->nStr = value;
  value = songReadByte(file, NULL); if(value < 0)return false; header->nPttn = value;
  value = songReadByte(file, NULL); if(value < 0)return false; header->maxSteps = value;
  value = songReadByte(file, NULL); if(value < 0)return false; header->nStrEncFnc = value;
  value = songReadByte(file, NULL); if(value < 0)return false; header->nStrPrsFnc = value;
  if(header->headerSize < songHeaderUsedSize)return false;
  for(uint16_t i = 0; i < header->headerSize - songHeaderUsedSize; i++){
    if(songReadByte(file, NULL) < 0)return false;
  }
  return true;
}

bool songValidateHeader(File& file, const struct SongFileHeader* header){
  if(header->version != songFormatVersion){
    lastSongStatus = SONG_DEFAULT_USED_BAD_VERSION;
    return false;
  }
  if(header->headerSize < songHeaderUsedSize || header->fileSize < header->headerSize || header->fileSize != file.size() || header->payloadSize != header->fileSize - header->headerSize){
    lastSongStatus = SONG_DEFAULT_USED_BAD_SIZE;
    return false;
  }
  if(header->nInst != genSq_nInst || header->nStr != nStrings || header->nPttn != genSq_nPttn || header->maxSteps != genSq_maxSteps || header->nStrEncFnc != genSq_nStrEncFnc || header->nStrPrsFnc != genSq_nStrPrsFnc){
    lastSongStatus = SONG_DEFAULT_USED_BAD_SIZE;
    return false;
  }
  return true;
}

bool songWriteChunkHeader(File& file, const char* id, struct SongChunkInfo* chunk, uint32_t* crc){
  if(!songWriteId(file, id, crc))return false;
  if(!songWriteU16(file, songFormatVersion, crc))return false;
  if(!songWriteU32(file, 0, NULL))return false;
  chunk->payloadStart = file.position();
  chunk->payloadLength = 0;
  return true;
}

bool songFinishChunk(File& file, struct SongChunkInfo* chunk){
  uint32_t payloadEnd = file.position();
  if(payloadEnd < chunk->payloadStart)return false;
  chunk->payloadLength = payloadEnd - chunk->payloadStart;
  if(!file.seek(chunk->payloadStart - 4))return false;
  if(!songWriteU32(file, chunk->payloadLength, NULL))return false;
  return file.seek(payloadEnd);
}

bool songReadChunkHeader(File& file, const char* expectedId, struct SongChunkInfo* chunk, uint32_t* crc){
  char id[4];
  uint16_t version;
  uint32_t length;
  if(!songReadId(file, id, crc))return false;
  if(!songReadU16(file, &version, crc))return false;
  if(!songReadU32(file, &length, NULL))return false;
  if(!songIdEquals(id, expectedId) || version != songFormatVersion){
    lastSongStatus = SONG_DEFAULT_USED_BAD_SIZE;
    return false;
  }
  chunk->payloadStart = file.position();
  chunk->payloadLength = length;
  if(chunk->payloadStart + chunk->payloadLength > file.size()){
    lastSongStatus = SONG_DEFAULT_USED_BAD_SIZE;
    return false;
  }
  return true;
}

bool songSkipBytes(File& file, uint32_t count, uint32_t* crc){
  for(uint32_t i = 0; i < count; i++){
    if(songReadByte(file, crc) < 0)return false;
  }
  return true;
}

bool songSkipChunkRemaining(File& file, const struct SongChunkInfo* chunk, uint32_t* crc){
  uint32_t payloadEnd = chunk->payloadStart + chunk->payloadLength;
  uint32_t current = file.position();
  if(current > payloadEnd){
    lastSongStatus = SONG_DEFAULT_USED_BAD_SIZE;
    return false;
  }
  return songSkipBytes(file, payloadEnd - current, crc);
}

bool songValueFitsByte(int value){
  return value >= 0 && value <= 255;
}

bool songCollectFromRuntime(struct SongData* song){
  if(!songValueFitsByte(rootNote) || !songValueFitsByte(scls_sclSel) || !songValueFitsByte(scls_sclStp) || !songValueFitsByte(scls_sclClr) || !songValueFitsByte(bpm) || !songValueFitsByte(scls_fledSrc))return false;
  song->rootNote = rootNote;
  song->sclSel = scls_sclSel;
  song->sclStp = scls_sclStp;
  song->sclClr = scls_sclClr;
  song->songBpm = bpm;
  song->fledSrc = scls_fledSrc;
  for(int inst = 0; inst < genSq_nInst; inst++){
    for(int str = 0; str < nStrings; str++){
      if(!songValueFitsByte(genSq_sclQ[inst][str]))return false;
      song->sclQ[inst][str] = genSq_sclQ[inst][str];
      for(int pttn = 0; pttn < genSq_nPttn; pttn++){
        for(int chFnc = 0; chFnc < genSq_nStrEncFnc; chFnc++){
          if(!songValueFitsByte(genSq_chn[inst][pttn][str][chFnc]))return false;
          song->chn[inst][pttn][str][chFnc] = genSq_chn[inst][pttn][str][chFnc];
        }
        for(int stp = 0; stp < genSq_maxSteps; stp++){
          if(!songValueFitsByte(genSq_stpOnOff[inst][pttn][str][stp]))return false;
          song->stpOnOff[inst][pttn][str][stp] = genSq_stpOnOff[inst][pttn][str][stp];
          for(int stpFnc = 0; stpFnc < genSq_nStrPrsFnc; stpFnc++){
            if(!songValueFitsByte(genSq_stp[inst][pttn][str][stp][stpFnc]))return false;
            song->stp[inst][pttn][str][stp][stpFnc] = genSq_stp[inst][pttn][str][stp][stpFnc];
          }
        }
      }
    }
  }
  for(int str = 0; str < nStrings; str++){
    if(!songValueFitsByte(tuning[str]) || !songValueFitsByte(strGain[str]))return false;
    song->songTuning[str] = tuning[str];
    song->songStrGain[str] = strGain[str];
  }
  return true;
}

bool songValidateData(const struct SongData* song){
  if(song->rootNote > 11)return false;
  if(song->sclSel >= nScales)return false;
  if(song->sclStp >= scls_numSclStp[song->sclSel] && scls_numSclStp[song->sclSel] > 0)return false;
  if(song->sclClr > 1)return false;
  if(song->songBpm < 1 || song->songBpm > 250)return false;
  for(int inst = 0; inst < genSq_nInst; inst++){
    for(int str = 0; str < nStrings; str++){
      if(song->sclQ[inst][str] > 1)return false;
      for(int pttn = 0; pttn < genSq_nPttn; pttn++){
        if(song->chn[inst][pttn][str][genSq_strEncFnc_tmDv] >= genSq_nTmDvs)return false;
        if(song->chn[inst][pttn][str][genSq_strEncFnc_stps] < 1 || song->chn[inst][pttn][str][genSq_strEncFnc_stps] > genSq_maxSteps)return false;
        if(song->chn[inst][pttn][str][genSq_strEncFnc_chn] > 16)return false;
        for(int stp = 0; stp < genSq_maxSteps; stp++){
          if(song->stpOnOff[inst][pttn][str][stp] > 1)return false;
          if(song->stp[inst][pttn][str][stp][genSq_strPrsFnc_sStp] > genSq_maxStpV[genSq_strPrsFnc_sStp])return false;
          if(song->stp[inst][pttn][str][stp][genSq_strPrsFnc_oct] > genSq_maxStpV[genSq_strPrsFnc_oct])return false;
          if(song->stp[inst][pttn][str][stp][genSq_strPrsFnc_vel] > 127)return false;
          if(song->stp[inst][pttn][str][stp][genSq_strPrsFnc_cc1] > 127)return false;
          if(song->stp[inst][pttn][str][stp][genSq_strPrsFnc_cc2] > 127)return false;
          if(song->stp[inst][pttn][str][stp][genSq_strPrsFnc_cc3] > 127)return false;
        }
      }
    }
  }
  for(int str = 0; str < nStrings; str++){
    if(song->songStrGain[str] > strGainMx)return false;
  }
  return true;
}

void songApply(const struct SongData* song){
  for(int inst = 0; inst < genSq_nInst; inst++){
    genSq_actPttn[inst] = 0;
    genSq_edtPttn[inst] = 0;
    for(int str = 0; str < nStrings; str++){
      genSq_sclQ[inst][str] = song->sclQ[inst][str];
      genSq_clk[inst][str] = -1;
      for(int pttn = 0; pttn < genSq_nPttn; pttn++){
        for(int chFnc = 0; chFnc < genSq_nStrEncFnc; chFnc++){
          genSq_chn[inst][pttn][str][chFnc] = song->chn[inst][pttn][str][chFnc];
        }
        for(int stp = 0; stp < genSq_maxSteps; stp++){
          genSq_stpOnOff[inst][pttn][str][stp] = song->stpOnOff[inst][pttn][str][stp];
          for(int stpFnc = 0; stpFnc < genSq_nStrPrsFnc; stpFnc++){
            genSq_stp[inst][pttn][str][stp][stpFnc] = song->stp[inst][pttn][str][stp][stpFnc];
          }
        }
        genSq_tmDv[inst][pttn][str] = genSq_tmDvs[genSq_chn[inst][pttn][str][genSq_strEncFnc_tmDv]];
      }
    }
  }
  rootNote = song->rootNote;
  scls_sclSel = song->sclSel;
  scls_sclStp = song->sclStp;
  scls_sclClr = song->sclClr;
  bpm = song->songBpm;
  chngBpm(bpm);
  scls_fledSrc = song->fledSrc;
  for(int str = 0; str < nStrings; str++){
    tuning[str] = song->songTuning[str];
  }
  for(int str = 0; str < nStrings; str++){
    strGain[str] = song->songStrGain[str];
    sndStrGain(str, strGain[str]);
  }
}

void songBuildDefault(struct SongData* song){
  for(int inst = 0; inst < genSq_nInst; inst++){
    for(int str = 0; str < nStrings; str++){
      song->sclQ[inst][str] = 0;
      for(int pttn = 0; pttn < genSq_nPttn; pttn++){
        song->chn[inst][pttn][str][genSq_strEncFnc_tmDv] = 10;
        song->chn[inst][pttn][str][genSq_strEncFnc_stps] = 16;
        song->chn[inst][pttn][str][genSq_strEncFnc_offSt] = 0;
        song->chn[inst][pttn][str][genSq_strEncFnc_sync] = 0;
        song->chn[inst][pttn][str][genSq_strEncFnc_chn] = inst + 1;
        for(int stp = 0; stp < genSq_maxSteps; stp++){
          song->stpOnOff[inst][pttn][str][stp] = 0;
          song->stp[inst][pttn][str][stp][genSq_strPrsFnc_sStp] = 0;
          song->stp[inst][pttn][str][stp][genSq_strPrsFnc_oct] = 3;
          song->stp[inst][pttn][str][stp][genSq_strPrsFnc_vel] = 40;
          song->stp[inst][pttn][str][stp][genSq_strPrsFnc_cc1] = 0;
          song->stp[inst][pttn][str][stp][genSq_strPrsFnc_cc2] = 0;
          song->stp[inst][pttn][str][stp][genSq_strPrsFnc_cc3] = 0;
        }
      }
    }
  }
  song->rootNote = 0;
  song->sclSel = 3;
  song->sclStp = 0;
  song->sclClr = 1;
  song->songBpm = 90;
  song->fledSrc = 2;
  for(int str = 0; str < nStrings; str++){
    song->songTuning[str] = defTuning[str];
    song->songStrGain[str] = defStrGain[str];
  }
}

void defaultSong(){
  songBuildDefault(&songBuffer);
  songApply(&songBuffer);
}

bool songWriteMeta(File& file, const struct SongData* song, uint32_t* crc){
  SongChunkInfo chunk;
  if(!songWriteChunkHeader(file, songMetaChunkId, &chunk, crc))return false;
  if(!songWriteByte(file, song->rootNote, crc))return false;
  if(!songWriteByte(file, song->sclSel, crc))return false;
  if(!songWriteByte(file, song->sclStp, crc))return false;
  if(!songWriteByte(file, song->sclClr, crc))return false;
  if(!songWriteByte(file, song->songBpm, crc))return false;
  if(!songWriteByte(file, song->fledSrc, crc))return false;
  return songFinishChunk(file, &chunk);
}

bool songWriteSeq(File& file, const struct SongData* song, uint32_t* crc){
  SongChunkInfo chunk;
  if(!songWriteChunkHeader(file, songSeqChunkId, &chunk, crc))return false;
  for(int inst = 0; inst < genSq_nInst; inst++){
    for(int str = 0; str < nStrings; str++){
      if(!songWriteByte(file, song->sclQ[inst][str], crc))return false;
      for(int pttn = 0; pttn < genSq_nPttn; pttn++){
        for(int chFnc = 0; chFnc < genSq_nStrEncFnc; chFnc++){
          if(!songWriteByte(file, song->chn[inst][pttn][str][chFnc], crc))return false;
        }
        for(int stp = 0; stp < genSq_maxSteps; stp++){
          if(!songWriteByte(file, song->stpOnOff[inst][pttn][str][stp], crc))return false;
          for(int stpFnc = 0; stpFnc < genSq_nStrPrsFnc; stpFnc++){
            if(!songWriteByte(file, song->stp[inst][pttn][str][stp][stpFnc], crc))return false;
          }
        }
      }
    }
  }
  return songFinishChunk(file, &chunk);
}

bool songWriteMix(File& file, const struct SongData* song, uint32_t* crc){
  SongChunkInfo chunk;
  if(!songWriteChunkHeader(file, songMixChunkId, &chunk, crc))return false;
  for(int str = 0; str < nStrings; str++){
    if(!songWriteByte(file, song->songTuning[str], crc))return false;
  }
  for(int str = 0; str < nStrings; str++){
    if(!songWriteByte(file, song->songStrGain[str], crc))return false;
  }
  return songFinishChunk(file, &chunk);
}

bool songWriteEnd(File& file, uint32_t* crc){
  SongChunkInfo chunk;
  if(!songWriteChunkHeader(file, songEndChunkId, &chunk, crc))return false;
  if(!songWriteU32(file, 0x444F4E45UL, crc))return false;
  return songFinishChunk(file, &chunk);
}

bool songWritePayload(File& file, const struct SongData* song, uint32_t* finalCrc){
  uint32_t crc = songCrcSeed;
  if(!songWriteMeta(file, song, &crc))return false;
  if(!songWriteSeq(file, song, &crc))return false;
  if(!songWriteMix(file, song, &crc))return false;
  if(!songWriteEnd(file, &crc))return false;
  *finalCrc = crc ^ songCrcFinalXor;
  return true;
}

bool songReadMeta(File& file, struct SongData* song, uint32_t* crc){
  SongChunkInfo chunk;
  if(!songReadChunkHeader(file, songMetaChunkId, &chunk, crc))return false;
  int value = songReadByte(file, crc); if(value < 0)return false; song->rootNote = value;
  value = songReadByte(file, crc); if(value < 0)return false; song->sclSel = value;
  value = songReadByte(file, crc); if(value < 0)return false; song->sclStp = value;
  if(chunk.payloadLength >= 6){
    value = songReadByte(file, crc); if(value < 0)return false; song->sclClr = value;
  } else {
    song->sclClr = 1;
  }
  value = songReadByte(file, crc); if(value < 0)return false; song->songBpm = value;
  value = songReadByte(file, crc); if(value < 0)return false; song->fledSrc = value;
  return songSkipChunkRemaining(file, &chunk, crc);
}

bool songReadSeq(File& file, struct SongData* song, uint32_t* crc){
  SongChunkInfo chunk;
  if(!songReadChunkHeader(file, songSeqChunkId, &chunk, crc))return false;
  for(int inst = 0; inst < genSq_nInst; inst++){
    for(int str = 0; str < nStrings; str++){
      int value = songReadByte(file, crc); if(value < 0)return false; song->sclQ[inst][str] = value;
      for(int pttn = 0; pttn < genSq_nPttn; pttn++){
        for(int chFnc = 0; chFnc < genSq_nStrEncFnc; chFnc++){
          value = songReadByte(file, crc); if(value < 0)return false; song->chn[inst][pttn][str][chFnc] = value;
        }
        for(int stp = 0; stp < genSq_maxSteps; stp++){
          value = songReadByte(file, crc); if(value < 0)return false; song->stpOnOff[inst][pttn][str][stp] = value;
          for(int stpFnc = 0; stpFnc < genSq_nStrPrsFnc; stpFnc++){
            value = songReadByte(file, crc); if(value < 0)return false; song->stp[inst][pttn][str][stp][stpFnc] = value;
          }
        }
      }
    }
  }
  return songSkipChunkRemaining(file, &chunk, crc);
}

bool songReadMix(File& file, struct SongData* song, uint32_t* crc){
  SongChunkInfo chunk;
  if(!songReadChunkHeader(file, songMixChunkId, &chunk, crc))return false;
  for(int str = 0; str < nStrings; str++){
    int value = songReadByte(file, crc); if(value < 0)return false; song->songTuning[str] = value;
  }
  for(int str = 0; str < nStrings; str++){
    int value = songReadByte(file, crc); if(value < 0)return false; song->songStrGain[str] = value;
  }
  return songSkipChunkRemaining(file, &chunk, crc);
}

bool songReadEnd(File& file, uint32_t* crc){
  uint32_t marker;
  SongChunkInfo chunk;
  if(!songReadChunkHeader(file, songEndChunkId, &chunk, crc))return false;
  if(!songReadU32(file, &marker, crc))return false;
  if(marker != 0x444F4E45UL){
    lastSongStatus = SONG_DEFAULT_USED_BAD_SIZE;
    return false;
  }
  return songSkipChunkRemaining(file, &chunk, crc);
}

bool songReadPayload(File& file, struct SongData* song, uint32_t expectedCrc){
  uint32_t crc = songCrcSeed;
  if(!songReadMeta(file, song, &crc))return false;
  if(!songReadSeq(file, song, &crc))return false;
  if(!songReadMix(file, song, &crc))return false;
  if(!songReadEnd(file, &crc))return false;
  crc ^= songCrcFinalXor;
  if(crc != expectedCrc){
    lastSongStatus = SONG_DEFAULT_USED_BAD_CRC;
    return false;
  }
  return true;
}

bool songLoadFile(int sng, struct SongData* song){
  char fileName[24];
  songFileName(sng, fileName, sizeof(fileName));
  if(!SD.exists(fileName)){
    lastSongStatus = SONG_DEFAULT_USED_MISSING;
    return false;
  }
  myFile = SD.open(fileName, FILE_READ);
  if(!myFile){
    lastSongStatus = SONG_DEFAULT_USED_MISSING;
    return false;
  }
  lastSongStatus = SONG_DEFAULT_USED_BAD_SIZE;
  SongFileHeader header;
  bool ok = songReadHeader(myFile, &header) && songValidateHeader(myFile, &header) && songReadPayload(myFile, song, header.crc32);
  if(ok && !songValidateData(song)){
    lastSongStatus = SONG_DEFAULT_USED_BAD_VALUE;
    ok = false;
  }
  if(ok)lastSongStatus = SONG_OK;
  myFile.close();
  return ok;
}

bool songWriteFile(int sng, const struct SongData* song){
  char fileName[24];
  char tempName[24];
  songFileName(sng, fileName, sizeof(fileName));
  songTempFileName(sng, tempName, sizeof(tempName));
  if(SD.exists(tempName))SD.remove(tempName);

  myFile = SD.open(tempName, FILE_WRITE);
  if(!myFile)return false;
  uint32_t crc;
  bool ok = songWriteHeader(myFile, songHeaderSize, 0, 0) && songWritePayload(myFile, song, &crc);
  if(ok){
    uint32_t fileSize = myFile.size();
    uint32_t payloadSize = fileSize - songHeaderSize;
    ok = myFile.seek(0) && songWriteHeader(myFile, fileSize, payloadSize, crc);
  }
  myFile.close();
  if(!ok){
    SD.remove(tempName);
    return false;
  }

  myFile = SD.open(tempName, FILE_READ);
  if(!myFile){
    SD.remove(tempName);
    return false;
  }
  SongFileHeader header;
  ok = songReadHeader(myFile, &header) && songValidateHeader(myFile, &header) && songReadPayload(myFile, &songVerifyBuffer, header.crc32) && songValidateData(&songVerifyBuffer);
  myFile.close();
  if(!ok){
    SD.remove(tempName);
    return false;
  }

  if(SD.exists(fileName) && !SD.remove(fileName)){
    SD.remove(tempName);
    return false;
  }
  if(!SD.rename(tempName, fileName)){
    SD.remove(tempName);
    return false;
  }
  return true;
}

void saveSong(int sng){
  lastSongSlot = sng;
  if(!songValidSlot(sng)){
    lastSongStatus = SONG_ERR_SLOT_RANGE;
    return;
  }
  if(!songCollectFromRuntime(&songBuffer) || !songValidateData(&songBuffer)){
    lastSongStatus = SONG_DEFAULT_USED_BAD_VALUE;
    return;
  }
  if(songWriteFile(sng, &songBuffer))lastSongStatus = SONG_OK;
  else lastSongStatus = SONG_SAVE_FAILED;
}

void loadSong(int sng){
  lastSongSlot = sng;
  if(!songValidSlot(sng)){
    defaultSong();
    lastSongStatus = SONG_ERR_SLOT_RANGE;
    return;
  }
  if(songLoadFile(sng, &songBuffer)){
    songApply(&songBuffer);
    lastSongStatus = SONG_OK;
  } else {
    SongStatus loadStatus = lastSongStatus;
    defaultSong();
    lastSongStatus = loadStatus;
  }
}

void scanPttns(){
  static unsigned int inter=3000;
  if (millis()-scanPttnTimer > inter){
    scanPttnTimer+=inter;
    inter=500;
    static int pttn = 0;
    if(pttn<=genSq_nPttn){
      genSq_actPttn[0]=pttn;
      genSq_edtPttn[0]=pttn;
      genSq_actPttn[1]=pttn;
      genSq_edtPttn[1]=pttn;
      genSq_actPttn[2]=pttn;
      genSq_edtPttn[2]=pttn;
      pttn++;
    }
    if(pttn==genSq_nPttn+1){
      genSq_actPttn[0]=0;
      genSq_edtPttn[0]=0;
      genSq_actPttn[1]=0;
      genSq_edtPttn[1]=0;
      genSq_actPttn[2]=0;
      genSq_edtPttn[2]=0;
      pttn++;
    }
  }
}

void rstAllSngs(){
  defaultSong();
  for(int i=0;i<genSq_nSngs;i++){
    saveSong(i);
  }
}
