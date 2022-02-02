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

AsciiMassagePacker outbound = new AsciiMassagePacker();


void setup() {
  size(512, 512);

  // Affiche les noms des ports.
  println(Serial.list());

  // Creer une instance de la classe Serial avec
  // une vitesse de 57600 baud:
  serial = new Serial(this, portName, 57600);
  
}




void draw() {

  
  float radians = map( mouseX, 0 , width, PI , TWO_PI );
  
  background(0);
  noFill();
  stroke(255);
  pushMatrix();
  translate(width*0.5, height*0.5);
  ellipse( 0, 0, 100, 100);
  rotate(radians);
  line(0,0,100,0);
  popMatrix();
  
  // CONVERT RADIANS TO ANGLE FOR THE SERVO:
  int angle = int( map( radians, PI, TWO_PI, 0, 180));
  
  // PACK ONE INT INTO AN "angle" MASSAGE:
  outbound.packOneInt("angle", angle);
  // PREVIOUS LINE IS THE SAME AS:
  // outbound.beginPacket("angle");
  // outbound.addInt(angle);
  // outbound.endPacket();
  
  // SEND THE PACKED MASSAGE:
  serial.write( outbound.buffer() );
  
}