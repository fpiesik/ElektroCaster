import processing.serial.*;
/*
Lorsque vous demarrez ce code, tout les ports
 series s'affichent dans la fenetre de deboguage.
 Changer la variable ''portName'' pour qu'elle
 corresponde au nom du port serie qui correspond a
 l'Arduino.
 
 Sous Linux et OS X le nom du port ressemble a ceci :
 String portName = "/dev/tty.usbmodem24121";
 
 Sous Windows, il ressemble a ceci :
 String portName = "COM4";
 */

String portName = "COM19";

// Declarer une instance de la classe Serial:
Serial serial;

AsciiMassageParser inbound = new AsciiMassageParser(this);
AsciiMassagePacker outbound = new AsciiMassagePacker();

long msFromArduino = 0;
int analogReading = 0;

int debugLed = 0;

void setup() {
  size(512, 256);

  // Affiche les noms des ports.
  println(Serial.list());

  // Creer une instance de la classe Serial avec
  // une vitesse de 57600 baud:
  serial = new Serial(this, portName, 57600);
  
}



void receivePacket() {
  while ( serial.available( ) > 0 ) {

    if ( inbound.parse( serial.read() ) ) {

      if ( inbound.fullMatch("ms") ) {
        msFromArduino = inbound.nextLong();
        println("Received ms: "+msFromArduino);
      } else if ( inbound.fullMatch("a0") ) {
        analogReading = inbound.nextInt();
        println("Received analog reading: "+analogReading);
      } else {
        println("Received unexpected massage");
      }
    }
  }
}

void draw() {

  receivePacket();

if ( debugLed == 1 ) {
  background(255);
  fill(0);
} else {
   background(0);
  fill(255);
}
  
  textAlign(CENTER, CENTER);
  text( "ms : " + msFromArduino, width * 0.5, height * 0.5 - 10);
  text( "a0 : " + analogReading, width * 0.5, height * 0.5 + 10);
}




void mousePressed() {

  if ( debugLed == 0 ) debugLed = 1;
  else debugLed = 0;

  

  outbound.beginPacket("d");
  outbound.addInt(debugLed);
  outbound.endPacket();
  serial.write( outbound.buffer() );
  
  println("Sent d "+debugLed);
  
}