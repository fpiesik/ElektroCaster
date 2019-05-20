static long mClock=0;

void midiClock() {
//sndClock(mClock);
//mClock++;
Serial5.write(202);
delayMicroseconds(waitS);
//sndNotes();
sndCC();
}

void midiStart() {
//mClock=-1;
Serial5.write(203);
delayMicroseconds(waitS);
//sndClock(mClock);
}

void midiContinue() {
  Serial.println("Continue");
}

void midiStop() {
Serial5.write(204);
delayMicroseconds(waitS);
//mClock=0;
//sndClock(mClock);
}
