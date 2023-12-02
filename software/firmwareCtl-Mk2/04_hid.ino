void readFretboard(int sensMode) {
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


  for (int s = 0; s < nStrings; s++) {
    strPrs[s] = 0;
    bool used = 0;
    for (int f = nFrets - 1; f >= 0; f--) {
      if (frtState[f][s] == 1 && used == 0) {
        if (f < nFrets)strPrs[s] = f + 1;
        //lastChng[s]=millis();
        used = 1;
      }
      lastFrtState[f][s] = frtState[f][s];
    }
    if (strPrs[s] == lastStrPrs[s])strBnc[s]++;
    if (strPrs[s] != lastStrPrs[s])strBnc[s] = 0;
    lastStrPrs[s] = strPrs[s];
  }

  //actions when string is pressed or released
  static long lastChng[nStrings];
  static long lastExStrPr[nStrings];
  bool frtSplt=opMode>=genSq_opMode && opMode<genSq_opMode+genSq_nInst;
  for (int s = 0; s < nStrings; s++) {
    unsigned int sB = strBnc[s];
    unsigned int sBncs = strBncs;
    if (shift==1 && fbrdMode == 0 && strPrs[s]>0 )strHold[s]=1;
    if (shift==1 && fbrdMode == 0 && strPrs[s]==0 )strHold[s]=0;
    if(frtSplt==1 && strPrs[s]>=frtSplit) sBncs=strBncsP; //extended string bounces threshold for switching patterns
    if (sB >= sBncs && millis() - lastChng[s] > fretMaskT && lastExStrPr[s] != strPrs[s]) {
      if (strHold[s]==0){
        if (fbrdMode == 0 && strArp_act == 0 && strSeq_act==0) {
          sndTrigEnv(s, strPrs[s]);
          if(opMode>=genSq_opMode && opMode<genSq_opMode+genSq_nInst && strPrs[s]<=frtSplit){
            if(frtb_sensMode==0)sndMidiNotePress(s,strPrs[s]);
            if (strPrs[s] > 0)kick(s);
          }
          else{
            if(frtb_sensMode==0)sndMidiNotePress(s,strPrs[s]);
            if (strPrs[s] > 0)kick(s);
          }
        }
        if (fbrdMode == 0)sndStrPrs(s, strPrs[s]);
      
      genSq_editSteps(s);
      //lastStrPrs[s]=strPrs[s];
      lastExStrPr[s] = strPrs[s];
    }
      lastChng[s] = millis();
      
      
    }
  }
}

void rcvHidD(byte idx, int val) {
  hidDVal[idx] = val;
  if (hidDVal[idx] != lastHidDVal[idx]) {
    procHidDChng(idx, val);
//    Serial.print("hidDVal: ");
//    Serial.print(idx);
//    Serial.print(" ");
//    Serial.println(hidDVal[idx]);
  }
}

void procHidDChng(byte idx, bool val) {
  switch (idx) {
    case 0:
      //display encoder button
      dispEncMode = !val;
//      Serial.print("dispEncMode: ");
//      Serial.println(dispEncMode);
      break;

    case 1:
      //button under display encoder
      shift = !val;
      break;

    case 2:
      //tripple switch left
      strArp_act = 0;
      strSeq_act = val;
      break;

    case 3:
      //tripple switch middle
      sndBowOn(val);
      break;

    case 4:
      //tripple switch right
      kickOn = val;
      break;

    case 5:
      //right tripple button
      //if (val == 1)loadSong();
      if (val == 1)schdSync = 1;
      break;

    case 6:
      //middle tripple button
      if (val == 1)clck_stp();
      break;

    case 7:
      //left tripple transport button
      //if (val == 1)saveSong();
      if (val == 1)clck_strt();
      break;

    case 8:
      //pots switch
      break;

    case 9:
      //embedden button near pots 
      if (val == 0 && shift == 1)defaultSong();
      if (val == 0 && shift == 0)loadSong(0);
      break;

    case 10:
      //pots Encoder button
      for (byte s = 0; s < (nStrings); s++) {
        genSq_chStrBtn(s, val);

      }
      break;

    case 11:
      fbrdMode = val; //switch next to display
      break;

    case 12:
      frtb_sensMode = val; //switch next to next switch to display
      break;
  }

  // string encoder buttons
  byte pO = 13;
  for (byte i = pO; i < (nStrings + pO); i++) {
    byte s = i - pO;
    if (hidDVal[pO + s] != lastHidDVal[pO + s]) {
      if(opMode>=genSq_opMode)genSq_chStrBtn(s, val);
      if(opMode==strArp_opMode)strArp_chStrBtn(s, val);
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
      sndMidiCC(midi_faderCc[0], val*127,mInst_chn);
      break;
    case 1:
      // fader 2
      sndMidiCC(midi_faderCc[1], val*127,mInst_chn);
      break;

    case 2:
      // fader 3
      sndMidiCC(midi_faderCc[2], val*127,mInst_chn);
      
      break;

    case 3:
      //fader 4
      sndMidiCC(midi_faderCc[3], val*127,mInst_chn);
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
void procHidRChng(byte idx, byte val) {
  switch (idx) {
    case 0:
      for (int s = 0; s < nStrings; s++) {
        strArp_tmDv[s] = strArp_tmDvs[val];
      }
//      Serial.print("tmDv");
//      Serial.println(strArp_tmDv[5]);
      break;

    case 1:
      genSq_actPttnsIdx=val;
      genSq_actPttnsCh();
      MIDI.sendNoteOn(val*2, 100, mM8Row_chn);
      MIDI.sendNoteOn(val, 0, mM8Row_chn);
//      saveSong();
//      genSq_actSng=val;
//      loadSong();
      break;

    case 2:
      chOpMode(val);
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
          if (fbrdMode == 0)strArp_chDispEnc(val);
          if (fbrdMode == 1)strArp_chDispEnc(val);
          break;
        case genSq_opMode:
          if (fbrdMode == 0)scls_chDispEnc(val);
          if (fbrdMode == 1)genSq_chDispEnc(val);
          break;
        case genSq_opMode+1:
          if (fbrdMode == 0)scls_chDispEnc(val);
          if (fbrdMode == 1)genSq_chDispEnc(val);
          break;
        case genSq_opMode+2:
          if (fbrdMode == 0)scls_chDispEnc(val);
          if (fbrdMode == 1)genSq_chDispEnc(val);
          break;
      }
    break;
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
              genSq_chStrEnc(s, val);
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
        genSq_chStrEnc(genSq_strPrsFnc, val);
      }

      break;
  }

  for (int s = 0; s < nStrings; s++) {
    if (hidEVal[s + 1] != lastHidEVal[s + 1]) {
      genSq_strEncChAStps = 0;
      lastStrEnc = s;
      switch (opMode) {
        case strSetup_opMode:
          strSetup_chStrEnc(s, val);
          break;
        case strArp_opMode:
          strArp_chStrEnc(s, val);
          break;
        case genSq_opMode:
          genSq_chStrEnc(s, val);
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
