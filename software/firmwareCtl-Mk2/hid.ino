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
  for (int s = 0; s < nStrings; s++) {
    unsigned int sB = strBnc[s];
    if (sB >= strBncs && millis() - lastChng[s] > fretMaskT && lastExStrPr[s] != strPrs[s]) {
      if (opStrMode == 1 && strArp_act == 0) {
        if (fbrdMode == 0)sndTrigEnv(s, strPrs[s]);
        if (strPrs[s] > 0) kick(s);
      }

      if (fbrdMode == 0)sndStrPrs(s, strPrs[s]);
      genSq_editSteps(s);
      //lastStrPrs[s]=strPrs[s];
      lastChng[s] = millis();
      lastExStrPr[s] = strPrs[s];
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
      strArp_act = val;
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
      break;

    case 10:
      //pots Encoder button
      for (byte s = 0; s < (nStrings); s++) {
        genSq_chStrBtn(s, val);

      }
      break;

    case 11:
      fbrdMode = val; //switch next to display
      if (val == 0)sndVol(vol);
      if (val == 1)sndVol(0);
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
      genSq_chStrBtn(s, val);
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
      sndLfo1(4, val); //if(hidAVal[0]!=lastHidAVal[0])sndEnv2(4,hidAVal[0]),lastHidAVal[0] = hidAVal[0];
      break;
    case 1:
      sndLfo1(3, val);
      break;

    case 2:
      sndLfo1(2, val);
      break;

    case 3:
      sndLfo1(1, val);
      break;

    case 4:
      sndEnv2(3, val);
      break;

    case 5:
      sndEnv1(3, val);
      break;

    case 6:
      sndEnv2(1, val);
      break;
    case 7:
      sndEnv1(1, val);
      break;

    case 8:
      sndEnv2(6, val);
      break;

    case 9:
      sndEnv1(6, val);
      break;

    case 10:
      break;

    case 11:
      break;

    case 12:
      //sndFX(1,hidAVal[12]);
      break;

    case 13:
      vol = val;
      if (fbrdMode == 0)sndVol(vol);
      break;

    case 14:
      sndFilter(1, val);
      break;

    case 15:
      sndFilter(3, val);
      break;

    case 16:
      //sndFX(0,val);
      break;

    case 17:
      //sndFX(3,val)
      break;

    case 18:
      //sndFX(2,val);
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
//      saveSong();
//      genSq_actSng=val;
//      loadSong();
      break;

    case 2:
      fledMode = opMdMap[val];
      opStrMode = opMdMap[val];
      //if(opStrMode==0||opStrMode==3)sndVol(0);
      //if(opStrMode==1||opStrMode==2)sndVol(vol);
      opMode = opMdMap[val];

      if (opMdMap[val] >= genSq_opMode && opMdMap[val] < genSq_opMode + genSq_nInst) {
        genSq_actInst = opMdMap[val] - genSq_opMode;
      }

      //opStrMode=opMdMap[val];
      //if(opStrMode==0||opStrMode==3)sndVol(0);
      //if(opStrMode==1||opStrMode==2)sndVol(vol);
//      Serial.print("genSq_actInst: ");
//      Serial.println(genSq_actInst);
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
      if (fbrdMode == 0) {
        switch (opMode) {
          case 0:
            bpm=bpm+val;
            if(bpm<1)bpm=1;
            if(bpm>250)bpm=250;
            chngBpm(bpm);
            break;
          case 1:
            strArp_chDispEnc(val);
            break;
          case 2:
            scls_chDispEnc(val);
            break;
          case 3:
            scls_chDispEnc(val);
            //genSq_chDispEnc(val);
            break;
          case 4:
            scls_chDispEnc(val);
            //genSq_chDispEnc(val);
            break;
          case 5:
            scls_chDispEnc(val);
            //genSq_chDispEnc(val);
            break;
        }
      }
      if (fbrdMode == 1) {
        switch (opMode) {
          case 0:
            bpm=bpm+val;
            if(bpm<1)bpm=1;
            if(bpm>250)bpm=250;
            chngBpm(bpm);
            break;
          case 1:
            strArp_chDispEnc(val);
            break;
          case 2:
            scls_chDispEnc(val);
            break;
          case 3:
            //scls_chDispEnc(val);
            genSq_chDispEnc(val);
            break;
          case 4:
            //scls_chDispEnc(val);
            genSq_chDispEnc(val);
            break;
          case 5:
            //scls_chDispEnc(val);
            genSq_chDispEnc(val);
            break;
        }
      }
      break;
    case 7:
      if (strEncMode == 0) {
        for (int s = 0; s < nStrings; s++) {
          switch (opMode) {
            case 0:
              tuning[s] = tuning[s] + val;
              break;
            case 1:
              strArp_chStrEnc(s, val);
              break;
            case 2:
              break;
            case 3:
              genSq_chStrEnc(s, val);
              break;
            case 4:
              genSq_chStrEnc(s, val);
              break;
            case 5:
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
        case 0:
          tuning[s] = tuning[s] + val;
          break;
        case 1:
          strArp_chStrEnc(s, val);
          break;
        case 2:
          break;
        case 3:
          genSq_chStrEnc(s, val);
          break;
        case 4:
          genSq_chStrEnc(s, val);
          break;
        case 5:
          genSq_chStrEnc(s, val);
          break;
      }
    }
  }
}
