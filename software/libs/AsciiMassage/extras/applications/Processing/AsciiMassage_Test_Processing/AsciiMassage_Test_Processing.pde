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

String portName = "COM17";

// Declarer une instance de la classe Serial:
Serial serial;

AsciiMassageParser inbound = new AsciiMassageParser(this);
AsciiMassagePacker outbound = new AsciiMassagePacker();

int byteValue; // Arduino byte = Processing int limited between 0 and 255
int intValue;
float floatValue;
int longValue; // Arduino long = Processing int

int receivedByte = 0;  // Arduino byte = Processing int limited between 0 and 255
int receivedInt = 0;
float receivedFloat = 0;
int receivedLong = 0; // Arduino long = Processing int

void setup() {
  size(512, 256);

  // Affiche les noms des ports.
  println(Serial.list());

  // Creer une instance de la classe Serial avec
  // une vitesse de 57600 baud:
  serial = new Serial(this, portName, 57600);
}



void draw() {

  background(0);

  if ( inbound.parseStream(serial) ) {

    if ( inbound.fullMatch("address") ) {
      // GET THE DATA
      receivedByte = inbound.nextByte();
      receivedInt = inbound.nextInt();
      receivedFloat = inbound.nextFloat();
      receivedLong = inbound.nextLong();
    } else {
      println("Received unexpected massage");
    }
  }

  text("Sent to Arduino", 10, 30 );
  text(byteValue, 10 , 50);
  text(intValue, 10 , 70);
  text(floatValue, 10 , 90);
  text(longValue, 10 , 110);

  text("Received from Arduino", width*0.5 , 30 );
  text(receivedByte, width*0.5 , 50);
  text(receivedInt, width*0.5 , 70);
  text(receivedFloat, width*0.5 , 90);
  text(receivedLong, width*0.5 , 110);
}




void mousePressed() {

  byteValue = (int) random(0, 255);
  intValue = (int) random(-32768, 32767);
  floatValue = random(-100, 100);
  longValue = (int) random(-2147483648, 2147483647);

  outbound.beginPacket("address");  /// Begins a packet.

  outbound.addByte(byteValue); /// Adds a byte.
  outbound.addInt(intValue); /// Adds an int.
  outbound.addFloat(floatValue); /// Adds a float.
  outbound.addLong(longValue); /// Adds a long.

  outbound.streamPacket(serial); /// Ends the massage and streams it.
}