// EXAMPLE FOR ESP8266

// INCLUDE ESP8266WiFi:
#include <ESP8266WiFi.h>

// A CYCLING 74 EXAMPLE TO COMMUNICATE WITH THIS SKETCH CAN BE FOUND INSIDE
// "applications/Cycling 74 Max 7/AsciiMassenger UDP.maxpat" OF THE FOLLOWING DOWNLOAD :
// https://github.com/SofaPirate/AsciiMassage/archive/master.zip

// Please change the following values with your network settings:
const char* ssid     = "******";
const char* password = "*****";
IPAddress ip(10, 5, 0, 120);
IPAddress gateway(10, 5, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// INCLUDE ESP8266 UDP
#include <WiFiUdp.h>
WiFiUDP udp;

// INCLUDE MASSAGE PARSER
#include <AsciiMassageParser.h>
// INCLUDE MASSAGE PACKER
#include <AsciiMassagePacker.h>

// INSTANCIATE PACKER(FOR SENDING) AND PARSER(FOR RECEIVING) INSTANCES.
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

// INCLUDE CHRONO SO WE SEND MESSAGES AT A FIXED INTERVAL.
#include <Chrono.h>
Chrono sendMassageChrono;

//===========
//== SETUP ==
//===========
void setup() {

  // INITIATE SERIAL COMMUNICATION FOR DEBUGGING.
  Serial.begin(57600);

  // [START] BEGIN WIFI
  WiFi.config(ip , gateway , subnet );
  WiFi.begin(ssid, password);
  // [END] BEGIN WIFI

  // [START] WAIT UNTIL CONNECTED
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(10);
  }
  // [END] WAIT UNTIL CONNECTED

  // [START] PRINT CONNECTION SETTINGS
  Serial.println();
  Serial.println("WiFi connected, IP address: ");
  Serial.println( WiFi.localIP() );
  // [END] PRINT CONNECTION SETTINGS


  udp.begin(7777); // BEGIN LISTENING ON UDP PORT 7777.

}


//==========
//== LOOP ==
//==========
void loop() {

 // REVEIVE MASSAGES
 // ================

  // [START] RECEIVE PACKETS OVER UDP
  if ( udp.parsePacket() ) {
    // [START] AN UDP PACKET WAS RECEIVED
    if ( inbound.parseStream( &udp ) ) {
      // [START] A MASSAGE WAS PARSED
      if ( inbound.fullMatch("address") ) {
        // [START] AN "address" MASSAGE WAS RECEIVED
        // do something with the data
        // [END] AN "address" MASSAGE WAS RECEIVED
      }
      // [END] A MASSAGE WAS PARSED
    }
    // [END] AN UDP PACKET WAS RECEIVED
  }
  // [END] RECEIVE PACKETS OVER UDP.

 // SEND MASSAGES
 // ================

  // [START] SEND PACKET OVER UDP EVERY 50 ms:
  if ( sendMassageChrono.hasPassed(50) ) { // THE CHRONO PASSED 50 ms.
    sendMassageChrono.restart(); // RESTART THE CHRONO.

    // [START] PREPARE ASCII PACKET
    outbound.beginPacket("address");
    outbound.addByte(195); // ADD WHAT YOU WANT.
    outbound.endPacket();
    // [END] PREPARE ASCII PACKET.

    // [START] SEND PACKET OVER UDP
    IPAddress targetIp = IPAddress(10, 5, 0, 141);
    int targetPort = 7890;
    udp.beginPacket( targetIp , targetPort );
    outbound.streamPacket(&udp);
    udp.endPacket();
    // [END] SEND PACKETS OVER UDP
  }
  // [END] SEND PACKET OVER UDP EVERY 50 ms
}


