void scanFretboardPins(int sensMode) {
  switch (sensMode) {
    case 0:
      for (int f = 0; f < nFrets; f++) {
        pinMode(frtPins[f], OUTPUT);
        digitalWrite(frtPins[f], LOW);
        for (int s = 0; s < nStrings; s++) {
          frtState[f][s] = !digitalRead(strPins[s]);
        }
        pinMode(frtPins[f], INPUT);
      }
      break;
    case 1:
      for (int f = 0; f < nFrets; f++) {
        pinMode(frtPins[f], OUTPUT);
        digitalWrite(frtPins[f], LOW);
      }
      for (int s = 0; s < nStrings; s++) {
        frtState[1][s] = !digitalRead(strPins[s]);
      }
  }
}

void updateFretDebounce() {
  for (int s = 0; s < nStrings; s++) {
    strPrs[s] = 0;
    bool used = 0;
    frtPrs[s] = 0;
    for (int f = nFrets - 1; f >= 0; f--) {
      if (frtState[f][s] == 1) {
        if (used == 0){
          strPrs[s] = f + 1;
          used = 1;
        }
        else {
          frtPrs[s]+=1;
        }
      }
      lastFrtState[f][s] = frtState[f][s];
    }
    if (strPrs[s] == lastStrPrs[s])strBnc[s]++;
    if (strPrs[s] != lastStrPrs[s])strBnc[s] = 0;
    lastStrPrs[s] = strPrs[s];
  }
}

bool emitFretEvents(int *eventString, int *eventPress) {
  static long lastChng[nStrings];
  static int lastExStrPr[nStrings];
  for (int s = 0; s < nStrings; s++) {
    unsigned int sB = strBnc[s];
    unsigned int sBncs = strBncs;
    if (shift==1 && fbrdMode == 0 && strPrs[s]>0 )strHold[s]=1;
    if (shift==1 && fbrdMode == 0 && strPrs[s]==0 )strHold[s]=0;
    //if(frtSplt==1 && strPrs[s]>=frtSplit) sBncs=strBncsP; //extended string bounces threshold for switching patterns
    if (sB >= sBncs && millis() - lastChng[s] > fretMaskT && lastExStrPr[s] != strPrs[s]) {
      *eventString = s;
      *eventPress = strPrs[s];
      lastExStrPr[s] = strPrs[s];
      lastChng[s] = millis();
      return 1;
    }
  }
  return 0;
}

void handleFretEventForAudioMidiKickSeq(int eventString, int eventPress, int sensMode) {
  int s = eventString;
  int press = eventPress;
  strArp_notePressOrder(s, press);
  int chnl;
  if(strArp_modeSel==0){
    chnl = genSq_chn[0][genSq_actPttn[0]][s][genSq_strEncFnc_chn];
    }
  else{
    chnl = strArp_chn[s];
    }

  if (strHold[s]==0||genSq_muteCh[0][s]){
    if (fbrdMode == 0 && strArp_act == 0 && strSeq_act==0) {
      sndTrigEnv(s, press>0);
      if(opMode>=genSq_opMode && opMode<genSq_opMode+genSq_nInst && press<=frtSplit){
        if(sensMode==0)sndMidiNotePress(s,press,chnl);
        if (press > 0)kick(s);
      }
      else{
        if(sensMode==0)sndMidiNotePress(s,press,chnl);
        if (press > 0)kick(s);
      }
    }
  }
  if (fbrdMode == 0 && press == 0)sndStrPrs(s, tuning[s],0);
  if (fbrdMode == 0 && press != 0)sndStrPrs(s, tuning[s] + press,1);
  if (press != 0 && fbrdMode == 0) lastNZStrPrs[s] = press;
  genSq_editSteps(s);
}

void readFretboard(int sensMode) {
  scanFretboardPins(sensMode);
  updateFretDebounce();

  //actions when string is pressed or released
  int eventString;
  int eventPress;
  while (emitFretEvents(&eventString, &eventPress)) {
    handleFretEventForAudioMidiKickSeq(eventString, eventPress, sensMode);
  }
}

void rcvHidD(byte idx, int val) {
  hidDVal[idx] = val;
  if (hidDVal[idx] != lastHidDVal[idx]) {
    procHidDChng(idx, val);
  }
}

void procHidDChng(byte idx, bool val) {
  switch (idx) {
    case 0:
      //display encoder button
      dispEncMode = !val;
      if (dispEncMode==0){
        for(int i=0;i<genSq_nInst;i++){
          if (genSq_edtPttn[i] != genSq_lastActPttn[i]){
            genSq_cpPttn(i, genSq_lastActPttn[i], i, genSq_edtPttn[i]);
          }
        }
      }
      if (dispEncMode==1){
        for(int i=0;i<genSq_nInst;i++){
          genSq_lastActPttn[i]=genSq_actPttn[i];
        }
      }
      break;

    case 1:
      //Shift: button under display encoder
      shift = !val;
      break;

    case 2:
      //tripple switch left
      strSeq_act = val && !strArp_modeSel;
      strArp_act = val && strArp_modeSel;
      break;

    case 3:
      //tripple switch middle
      sndBowOn(val);
      if (val == 0 && shift == 1 )extClk = val;
      if (val == 1 && shift == 1 )extClk = val;
      break;

    case 4:
      //tripple switch right
      kickOn = val;
      if (val == 0 && shift == 1 )frtb_sensMode = val;
      if (val == 1 && shift == 1 )frtb_sensMode = val;
      break;

    case 5:
      //right tripple button
      if (val == 1){
        schdSync[0] = 1;
        schdSync[1] = 1;
        schdSync[2] = 1;
      }
      break;

    case 6:
      //middle tripple button
      if (val == 0){
        tgl_ply=!tgl_ply;
        if (tgl_ply==1)clck_strt();
        if (tgl_ply==0)clck_stp();
      }      
      break;

    case 7:
      //left tripple button
      if (val == 1){
        genSq_actInst=(genSq_actInst+1)%genSq_nInst;
        //if(genSq_actInst==0 && strArp_modeSel)chOpMode(strArp_opMode);
        chOpMode(genSq_actInst+genSq_opMode);
      }
      break;

    case 8:
      //pots switch
      genSq_strEncBtnSw = val;
      break;

    case 9:
      //embedded button near pots 
      if (val == 0 && shift == 1 && dispEncMode == 1)defaultSong();
      if (val == 0 && shift == 0 && dispEncMode == 1)loadSong(0);
      if (val == 0 && shift == 0 && dispEncMode == 0)loadSong(genSq_actSng);
      if (val == 0 && shift == 1 && dispEncMode == 0)saveSong(genSq_actSng);
      break;

    case 10:
      //pots Encoder button
      for (byte s = 0; s < (nStrings); s++) {
        genSq_chStrBtn(s, val);

      }
      break;

    case 11:
    //switch next to display
      fbrdMode = val; 
      break;

    case 12://switch next to next switch to display
      //frtb_sensMode = val; 
      fbrdSeqVHld=val; //see the sequencer while playing (editing disabled)
      break;
  }

  // string encoder buttons
  byte pO = 13;
  for (byte i = pO; i < (nStrings + pO); i++) {
    byte s = i - pO;
    if (genSq_strEncBtnSw == 1){
      if (hidDVal[pO + s] != lastHidDVal[pO + s]) {
        if(opMode>genSq_opMode)genSq_strEncFnc=nStrings-s-1;
        if(opMode==genSq_opMode && strArp_modeSel==0)genSq_strEncFnc=nStrings-s-1;
        if(opMode==genSq_opMode && strArp_modeSel==1)strArp_strEncFnc=nStrings-s-1;
      }
    }
    if (genSq_strEncBtnSw == 0){
      if (hidDVal[pO + s] != lastHidDVal[pO + s]) {
        if(opMode>genSq_opMode)genSq_chStrBtn(s, val);
        if(opMode==genSq_opMode && strArp_modeSel==0)genSq_chStrBtn(s, val);
        if(opMode==genSq_opMode && strArp_modeSel==1)strArp_chStrBtn(s, val);
      }
    }
  }

  lastHidDVal[idx] = hidDVal[idx];
}

void rcvHidA(byte idx, int dVal) {
  float val = dVal / 200.0;
  hidAVal[idx] = val;
  if (hidAVal[idx] != lastHidAVal[idx]) {
    procHidAChng(idx, val);
    lastHidAVal[idx] = hidAVal[idx];
  }
}
void procHidAChng(byte idx, float val) {
  switch (idx) {
    case 0:
      // fader 1
      val = val * 130;
      if (val>127) val = 127;
      sndMidiCC(midi_faderCc[0], val,mInst_chn);
      break;
    case 1:
      // fader 2
      val = val * 130;
      if (val>127) val = 127;
      sndMidiCC(midi_faderCc[1], val,mInst_chn);
      break;

    case 2:
      // fader 3
      val = val * 130;
      if (val>127) val = 127;
      sndMidiCC(midi_faderCc[2], val,mInst_chn);
      
      break;

    case 3:
      //fader 4
      val = val * 130;
      if (val>127) val = 127;
      sndMidiCC(midi_faderCc[3], val,mInst_chn);
      break;

    case 4:
      // fader 5
      sndEnv2(3, val);
      break;

    case 5:
      // fader 6
      sndEnv1(3, val);
      break;

    case 6:
      //fader 7
      sndEnv2(1, val);
      break;
    case 7:
      // fader 8
      sndEnv1(1, val);
      break;

    case 8:
      //fader 9
      sndEnv2(6, val);
      break;

    case 9:
      //fader 10
      sndEnv1(6, val);
      break;

    case 10:
      //joystick X
      sndMidiCC(mInst_anaXyCc[0], val*127,mInst_chn);
      break;

    case 11:
      //joystick Y
      sndMidiCC(mInst_anaXyCc[1], val*127,mInst_chn);
      usbMIDI.sendAfterTouch(val*127, mInst_chn);
      break;

    case 12:
      //pot 7
      //sndMidiCC(mInst_potCc[0], val*127,mInst_chn);
      sndLfo1(2, val);
      break;

    case 13:
      //pot 1
      vol = val;
      if (mtOut == 0)sndVol(vol);
      break;

    case 14:
      //pot 2
      sndFilter(1, val);
      break;

    case 15:
      //pot 3
      sndFilter(3, val);
      break;

    case 16:
      sndLfo1(3, val);
      break;

    case 17:
      //pot 5
      //sndFX(3,val)
      sndMidiCC(mInst_potCc[0], val*127,mInst_chn);
      break;

    case 18:
      //pot 6
      //sndFX(2,val);
      sndLfo1(1, val);
      break;
  }
  lastHidAVal[idx] = hidAVal[idx];
}

void rcvHidR(byte idx, int val) {
  hidRVal[idx] = val;
  if (hidRVal[idx] != lastHidRVal[idx])procHidRChng(idx, val);
}
void updStrAutoMode(){
  bool autoTrig = hidDVal[2];
  strSeq_act = autoTrig && !strArp_modeSel;
  strArp_act = autoTrig && strArp_modeSel;
}

void procHidRChng(byte idx, byte val) {
  switch (idx) {
    case 0:
      if(val<genSq_nPttn){
        schdStrArpModeSel = 0;
        //if(opMode==strArp_opMode)chOpMode(genSq_opMode);
        if(shift==0){
          schdPttnCh[idx]=val;
          for (int i=0;i<genSq_nInst;i++){
            if(genSq_syncInst[i]==idx){
              schdPttnCh[i]=val;
              genSq_edtPttn[i]=val;
            }
          }
        }
        genSq_edtPttn[idx]=val;
      }
      if(val<genSq_nPttn){
        for (int i=0;i<genSq_nInst;i++){
          if(val==11-i)genSq_syncInst[idx]=i;
          if(val<=11-genSq_nInst)genSq_syncInst[idx]=-1;
        }
      }
      if(val>=genSq_nPttn){
        strArp_modeVal = val;
        schdStrArpModeSel = 1;
        schdStrArpPttnCh = 11 - val;
        if(schdStrArpPttnCh >= strArp_nPttn)schdStrArpPttnCh = 0;
        //if(opMode==genSq_opMode)chOpMode(strArp_opMode);
      }
      break;

    case 1:
      if(val<genSq_nPttn){
        if(shift==0){
          schdPttnCh[idx]=val;
          for (int i=0;i<genSq_nInst;i++){
            if(genSq_syncInst[i]==idx){
              schdPttnCh[i]=val;
              genSq_edtPttn[i]=val;
            }
          }
        }
        genSq_edtPttn[idx]=val;
      }
      for (int i=0;i<genSq_nInst;i++){
        if(val==11-i)genSq_syncInst[idx]=i;
        if(val<=11-genSq_nInst)genSq_syncInst[idx]=-1;
      }
      if(val==genSq_nPttn)chOpMode(0);
      break;

    case 2:
      if(val<genSq_nPttn){
        if(shift==0){
          schdPttnCh[idx]=val;
          for (int i=0;i<genSq_nInst;i++){
            if(genSq_syncInst[i]==idx){
              schdPttnCh[i]=val;
              genSq_edtPttn[i]=val;
            }
          }
        }
        genSq_edtPttn[idx]=val;
      }
      for (int i=0;i<genSq_nInst;i++){
        if(val==11-i)genSq_syncInst[idx]=i;
        if(val<=11-genSq_nInst)genSq_syncInst[idx]=-1;
      }
      if(val==genSq_nPttn)chOpMode(0);
      break;
  }
  lastHidRVal[idx] = hidRVal[idx];
}

void rcvHidE(byte idx, long val) {
  hidEVal4[idx] = hidEVal4[idx] + val;
  hidEVal[idx] = hidEVal4[idx] / 4;
  if (hidEVal[idx] != lastHidEVal[idx]) {
    val = hidEVal[idx] - lastHidEVal[idx];
    procHidEChng(idx, val);
//    Serial.print("hidEVal: ");
//    Serial.print(idx);
//    Serial.print(" ");
//    Serial.println(hidEVal[idx]);
    lastHidEVal[idx] = hidEVal[idx];
  }
}

void procHidEChng(byte idx, long val) {
  switch (idx) {
    case 0:
      switch (opMode) {
        case strSetup_opMode:

          strSetup_chDispEnc(val);
          break;
        case strArp_opMode:
          if (fbrdMode==0&&fbrdSeqVHld==0)strArp_chDispEnc(val);
          if (fbrdMode==1||fbrdSeqVHld==1)strArp_chDispEnc(val);
          break;
        case genSq_opMode:
          if (fbrdMode==0&&fbrdSeqVHld==0&&strArp_modeSel==0)scls_chDispEnc(val);
          if ((fbrdMode==1||fbrdSeqVHld==1)&&strArp_modeSel==0)genSq_chDispEnc(val);
          if (fbrdMode==0&&fbrdSeqVHld==0&&strArp_modeSel==1)scls_chDispEnc(val);
          if ((fbrdMode==1||fbrdSeqVHld==1)&&strArp_modeSel==1)strArp_chDispEnc(val);
          break;
        case genSq_opMode+1:
          if (fbrdMode==0&&fbrdSeqVHld==0)scls_chDispEnc(val);
          if (fbrdMode==1||fbrdSeqVHld==1)genSq_chDispEnc(val);
          break;
        case genSq_opMode+2:
          if (fbrdMode==0&&fbrdSeqVHld==0)scls_chDispEnc(val);
          if (fbrdMode==1||fbrdSeqVHld==1)genSq_chDispEnc(val);
          break;
      }
    break;
    //change all strings at once
    case 7:
      if (strEncMode == 0) {
        for (int s = 0; s < nStrings; s++) {
          switch (opMode) {
            case strSetup_opMode:
              tuning[s] = tuning[s] + val;
              break;
            case strArp_opMode:
              strArp_chStrEnc(s, val);
              break;
            case genSq_opMode:
              if(strHold[s]==0&&strArp_modeSel==0) genSq_chStrEnc(s, val); //change only if string is not hold
              if(strArp_modeSel==1)strArp_chStrEnc(s, val);
              break;
            case genSq_opMode+1:
              genSq_chStrEnc(s, val);
              break;
            case genSq_opMode+2:
              genSq_chStrEnc(s, val);
              break;
          }
        }
      }
      if (strEncMode == 1) {
        genSq_strEncChAStps = 1;
        genSq_chStrEnc(nStrings-genSq_strPrsFnc-1, val);
      }

      break;
  }

  for (int s = 0; s < nStrings; s++) {
    if (hidEVal[s + 1] != lastHidEVal[s + 1]) {
      genSq_strEncChAStps = 0;
      for (int s = 0; s < nStrings; s++) {
        if(frtPrs[s]>1){
          genSq_strEncChAStps=1;
        }
      }
      lastStrEnc = s;
      switch (opMode) {
        case strSetup_opMode:
          strSetup_chStrEnc(s, val);
          break;
        case strArp_opMode:
          strArp_chStrEnc(s, val);
          break;
        case genSq_opMode:
          if(strArp_modeSel==0)genSq_chStrEnc(s, val);
          if(strArp_modeSel==1)strArp_chStrEnc(s, val);
          break;
        case genSq_opMode+1:
          genSq_chStrEnc(s, val);
          break;
        case genSq_opMode+2:
          genSq_chStrEnc(s, val);
          break;
      }
    }
  }
}

void chOpMode(int val){
  opMode = val;
  if (opMode >= genSq_opMode && opMode < genSq_opMode + genSq_nInst) genSq_actInst = opMode - genSq_opMode;
}
