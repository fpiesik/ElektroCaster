// This example uses :
// * the ASCII format for packing and parsing;
// * the Serial protocol for sending and receiving.

// It echoes back the string that follows an "echo" massage
// followed by the length of the received string.


// INCLUDE ASCII MASSAGE
#include <AsciiMassage.h>


// PACKER(FOR SENDING) AND PARSER(FOR RECEIVING).
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

// CREATE AN EMPTY STRING TO HOLD THE RECEIVED STRING.
// THIS EMPTY STRING CAN HOLD UP TO 63 CHARACTERS 
// (THE LAST INDEX IS THE NULL CHARACTER THAT ENDS 
// THE STRING).
#define BUFFER_LENGTH 64
char receivedString[BUFFER_LENGTH];


void setup() {

  // Start the Serial protocol at 57600 baud.
  Serial.begin(57600);

}

void loop() {

  while ( inbound.parseStream( &Serial) ) {
    // PARSE INPUT. RETURNS 1 (TRUE) IF MASSAGE IS COMPLETE.

      if ( inbound.fullMatch("echo") ) {

        // COPY THE RECEIVED STRING:
        int stringLength = inbound.nextString(receivedString,BUFFER_LENGTH);

        // ECHO THE RECEIVED STRING:
        outbound.beginPacket("back"); // Start a packet with the address called "back".
        outbound.addString(receivedString);
        outbound.addInt(stringLength);
        outbound.streamPacket(&Serial); // End and stream the packet.

        // SEND ANOTHER STRING:
        outbound.beginPacket("test"); // Start a packet with the address called "test".
        outbound.addString("this is a string");
        outbound.streamPacket(&Serial); // End and stream the psacket.

        
        
      } else {
        // SEND "what?" WHEN A MASSAGE IS NOT RECOGNIZED.
        outbound.streamEmpty(&Serial,"what?");
        
      }
    
  }

  // You can view the sent massages in Arduino's Serial Monitor
  // because Arduino's Serial Monitor uses the ASCII format.


}
