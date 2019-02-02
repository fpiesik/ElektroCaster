static long mClock=0;

void midiClock() {
sndClock(mClock);
mClock++;
}

void midiStart() {
mClock=-1;
//sndClock(mClock);
}

void midiContinue() {
  Serial.println("Continue");
}

void midiStop() {
mClock=0;
sndClock(mClock);
}
