void sndMidiNote(byte str,byte fret){
    if(fret>0){
      //usbMIDI.sendNoteOff(lastStrState[str]+tuning[str], 0, 2, 0);
      //usbMIDI.sendNoteOn(fret+tuning[str], 127, 2, 0);
    }
    if(fret==0){
      //usbMIDI.sendNoteOff(lastStrState[str]+tuning[str], 0, 2, 0);
    }
}
