// CIRCUIT :
// Attach a servo to digital pin 9.
// Attach a LED to digital pin 6.
// Attach a potentiometer or light sensor to analog pin 0.
// Attach a switch to digital pin 2.
// Circuit image : 
// https://github.com/SofaPirate/AsciiMassenger/blob/master/examples/AsciiMassenger_example/AsciiMassenger_example_circuit.png

#include <AsciiMassenger.h>
AsciiMassenger msg; // By default, AsciiMassenger uses Serial.

#include <Servo.h>
// Initialize a Servo :
Servo servoA;


void setup() {
  // Initialize serial communication :
  Serial.begin(57600);

  // Attach the servo to digital pin 9 :
  servoA.attach(9);

  // Attach a LED to digital pin 6  :
  pinMode(6, OUTPUT);

  // Attach a switch to digital pin  2 :
  pinMode(2, INPUT_PULLUP);

  // Debug LED (Indicated by a 'L' on the board)  :
  pinMode(13,OUTPUT);

  // Attach a potentiometer or light sensor to analog pin 0 :
  // Nothing to setup for the analog pin.
}

void loop() {

  while ( msg.parsePacket() ) { // Run as long as  there is a new Massage.
    msg.dispatch("/s", setServo); // If "/s" is received, execute setServo().
    msg.dispatch("/l", setLed); // If "/l" is received, execute setLed().
    msg.dispatch("/d", setDebugLed); // If "/d" is received, execute setDebugLed().
  }

  // Send the switch value :
  msg.beginPacket("/r");
  msg.addInt( digitalRead(2) );
  msg.endPacket();

  // Send the potentiometer or light sensor value :
  msg.beginPacket("/a");
  msg.addInt( analogRead(0) );
  msg.endPacket();

  delay(10); // 10 ms pause
}

// Set the servo angle :
void setServo() {
  int value = msg.nextInt();
  servoA.write(value);
}

// Set the LED fade :
void setLed() {
  int value = msg.nextInt();
  analogWrite(6, value);
}

// Set the debug LED value :
void setDebugLed() {
  int value = msg.nextInt();
  digitalWrite(13, value);
}