// A PROCESSING EXAMPLE TO COMMUNICATE WITH THIS SKETCH CAN BE FOUND  INSIDE
// "extras/applications/Processing/AsciiMassage_Processing/" OF THE FOLLOWING DOWNLOAD :
// https://github.com/SofaPirate/AsciiMassage/archive/master.zip

// A CYCLING 74 EXAMPLE TO COMMUNICATE WITH THIS SKETCH CAN BE FOUND INSIDE
// "extras/applications/Cycling 74 Max 7/AsciiMassenger.maxpat" OF THE FOLLOWING DOWNLOAD :
// https://github.com/SofaPirate/AsciiMassage/archive/master.zip


// INCLUDE MASSAGE
#include <AsciiMassagePacker.h>
#include <AsciiMassageParser.h>

// PACKER(FOR SENDING) AND PARSER(FOR RECEIVING) INSTANCES.
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

// THIS VARIABLE IS USED TO SLOW DOWN THE SENDING OF THE MASSAGES.
unsigned long lastTimeMassageSent;


///////////
// SETUP //
///////////
void setup() {

  // INITIATE SERIAL COMMUNICATION.
  Serial.begin(57600);

}



//////////
// LOOP //
//////////
void loop() {

  // PARSE SERIAL INPUT. RETURNS 1 (TRUE) IF A COMPLETE
  // MASSAGE WAS RECEIVED:
  if ( inbound.parseStream( &Serial ) ) {
    // DOES THE MASSAGE START WITH THE WORD "address"?
    if ( inbound.fullMatch("address") ) {
      byte byteValue = inbound.nextByte();
      int intValue = inbound.nextInt();
      float floatValue = inbound.nextFloat();
      long longValue = inbound.nextLong();
    } else {
      // SEND "what?" BECAUSE WE RECEIVED SOMETHING ELSE THAN "address"
      // THE MASSAGE IS CONSIDERED EMPTY BECAUSE IT DOES NOT HAVE ANY ARGUMENTS
      outbound.streamEmpty(&Serial, "what?");

    }
  }

  // SEND A PACKET EVERY 100 ms:
  if ( millis() - lastTimeMassageSent >= 100 ) {
    lastTimeMassageSent = millis();

    // BUILD A PACKET:

    /// Begins the sending of a message.
    outbound.beginPacket("address");
  
    /// Adds a byte.
    outbound.addByte(45);
  
    /// Adds an int.
    outbound.addInt(1024);
  
    /// Adds a long.
    outbound.addLong(64823);
  
    /// Adds a float.
    outbound.addFloat(183.92);
  
    /// End and stream the massage packet:
    outbound.streamPacket(&Serial);
  }
 

}



