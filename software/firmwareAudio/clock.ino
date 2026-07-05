void midiClock() {
//sndClock(mClock);
//mClock++;
Serial1.write(202);
delayMicroseconds(waitS);
//sndNotes();
sndCC();
}

void midiStart() {
//mClock=-1;
Serial1.write(203);
delayMicroseconds(waitS);
//sndClock(mClock);
}

void midiContinue() {
  //Serial.println("Continue");
}

void midiStop() {
Serial1.write(204);
delayMicroseconds(waitS);
//mClock=0;
//sndClock(mClock);
}
