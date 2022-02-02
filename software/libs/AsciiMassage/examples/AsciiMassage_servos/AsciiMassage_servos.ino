// THIS EXAMPLE SHOWS HOW TO CONTROL SERVOMOTORS ATTACHED TO PINS 9 AND 11
// WITH ASCIIMASSAGE.
// THE "s1" MASSAGE WILL CONTROL THE ANGLE OF THE FIRST SERVO.
// THE "s2" MASSAGE WILL CONTROL THE ANGLE OF THE SECOND SERVO.

// A PROCESSING EXAMPLE TO COMMUNICATE WITH THIS SKETCH CAN BE FOUND  INSIDE
// "extras/applications/Processing/" OF THE FOLLOWING DOWNLOAD :
// https://github.com/SofaPirate/AsciiMassage/archive/master.zip

// A CYCLING 74 EXAMPLE TO COMMUNICATE WITH THIS SKETCH CAN BE FOUND INSIDE
// "extras/applications/Cycling 74 Max 7/" OF THE FOLLOWING DOWNLOAD :
// https://github.com/SofaPirate/AsciiMassage/archive/master.zip

// INCLUDE MASSAGE
#include <AsciiMassagePacker.h>
#include <AsciiMassageParser.h>

// PACKER(FOR SENDING) AND PARSER(FOR RECEIVING) INSTANCES.
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

// INCLUDE SERVO.
#include <Servo.h>

// INSTANTIATE A SERVO OBJECT.
Servo servo1;

// INSTANTIATE A SERVO OBJECT.
Servo servo2;


///////////
// SETUP //
///////////
void setup() {

  // INITIATE SERIAL COMMUNICATION.
  Serial.begin(57600);

  // ATTACH THE SERVOS TO PINS 9 AND 11:
  servo1.attach(9);
  servo2.attach(11);

  pinMode(13,OUTPUT);

}



//////////
// LOOP //
//////////
void loop() {

    // PARSE INPUT. RETURNS 1 (TRUE) IF MASSAGE IS COMPLETE.
    if ( inbound.parseStream( &Serial ) ) {

      // CHANGE DEBUG LED STATE WHEN A MASSAGE IS RECEIVED.
      digitalWrite(13, !digitalRead(13));

      // THE "s1" MASSAGE WILL CONTROL THE ANGLE OF THE SERVO IN DEGREES.
      if ( inbound.fullMatch("s1") ) {

        int value = inbound.nextInt();
        servo1.write( value );

      // THE "s2" MASSAGE WILL CONTROL THE ANGLE OF THE SERVO IN MICROSECONDS.
      } else if ( inbound.fullMatch("s2") ) {

        int value = inbound.nextInt();
        servo2.write( value );

      // SEND "what?" WHEN A MASSAGE IS NOT RECOGNIZED.
      } else {
        outbound.streamEmpty("what?");
      }
    }

}




