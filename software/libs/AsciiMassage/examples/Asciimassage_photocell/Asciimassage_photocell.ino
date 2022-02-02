// ICLUDE Chrono (INSTALL Chrono WITH ARDUINO'S LIBRARY MANAGER )
#include <Chrono.h>

// Chrono USED TO SEND A MASSAGE EVERY 100 ms:
Chrono messageSendChrono = Chrono();

// ICLUDE MASSAGE PACKER
#include <AsciiMassagePacker.h>

// PACKER(FOR SENDING) INSTANCE:
AsciiMassagePacker outbound;

void setup() {

  // INITIATE SERIAL COMMUNICATION.
  Serial.begin(57600);

}

void loop() {
  
  // SEND A MASSAGE EVERY 100 ms:
  if ( messageSendChrono.hasPassed(100) ) {
    messageSendChrono.restart();

    int photocellReading = analogRead( A0 );

    outbound.beginPacket("photo");
    outbound.addInt( photocellReading );
    
    outbound.streamPacket(&Serial);

  }

}
