#include <AsciiMassagePacker.h>
#include <AsciiMassageParser.h>


// Instantiate Massage objects for encoding and decoding massages
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

bool sendMillis = false;
unsigned long lastTimeSentMillis;

void setup() {

  // Initiate serial communication.
  Serial.begin(57600);

  // Setup pin 13 as a digital output.
  pinMode(13, OUTPUT);

}

void loop() {

    // PARSE INPUT AND EXECUTRE massageReceived IF A COMPLETE MASSAGE IS RECEIVED 
    inbound.parseStream( &Serial, massageReceived );
  
  if ( sendMillis && lastTimeSentMillis - millis() >= 100) {

    outbound.streamOneLong("ms",millis() );
    
  }

}

// Process received massages.
void massageReceived() {

  if ( inbound.fullMatch("echo")) {
    int i = inbound.nextInt(); // ...read the next element as an int...
    float f = inbound.nextFloat();
    byte b = inbound.nextByte();

    outbound.beginPacket("echo");
    outbound.addInt(i);
    outbound.addFloat(f);
    outbound.addByte(b);
    outbound.streamPacket(&Serial);
     
  } else if ( inbound.fullMatch("ms") ) {
    
    sendMillis = inbound.nextInt();
    
  } else {
    
    outbound.streamEmpty(&Serial, "what?");
    
  }
}
