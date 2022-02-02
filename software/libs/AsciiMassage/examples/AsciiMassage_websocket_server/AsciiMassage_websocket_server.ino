// EXAMPLE FOR ESP8266

// A p5.js EXAMPLE TO COMMUNICATE WITH THIS SKETCH CAN BE FOUND INSIDE
// "applications/Websocket Client(p5.js)" OF THE FOLLOWING DOWNLOAD :
// https://github.com/SofaPirate/AsciiMassage/archive/master.zip

// THIS EXAMPLE USES THE DEBUG LED PIN FOR THE WEMOS D1 MINI (https://wiki.wemos.cc/products:d1:d1_mini).
// CHANGE THIS PIN TO FIT YOUR MODEL.
int debugLedPin = 2;

// INCLUDE THE CODE IN THE esp8266AP.h TAB.
#include "esp8266AP.h"

// INCLUDE WebSockets BY Markus Sattler FOR ESP8266.
// INSTALL THROUGH ARDUINO'S LIBRARY MANAGER.
#include <WebSocketsServer.h>

// CREATE A SERVER THAT LISTENS ON PORT 81.
WebSocketsServer webSocket = WebSocketsServer(81);

// INCLUDE MASSAGE
#include <AsciiMassagePacker.h>
#include <AsciiMassageParser.h>

// PACKER(FOR SENDING) AND PARSER(FOR RECEIVING) INSTANCES.
AsciiMassageParser inbound;
AsciiMassagePacker outbound;

bool sendAnalog = true;
unsigned long lastTimeSentAnalogReading = 0;

bool sendMs = true;
unsigned long lastTimeSentMs = 0;

//////////////////////
// SEND AND RECEIVE //
//////////////////////
// THE FOLLOWING FUNCTIONS ARE HELPER FUNCTIONS.
// sendPacket() SENDS OUT A MASSAGE ONCE IT IS PACKED.
// receivePacket() CHECK FOR A COMPLETED MASSAGE AND
// INDICATES WHAT TO DO WITH ITS CONTENTS.

// SEND PACKED PACKET OVER WEBSOCKET.
void sendPacket() {
  webSocket.broadcastTXT(outbound.buffer(), outbound.size());
}

// RECEIVE OVER WEBSOCKET AND PARSE ASCII PACKET.
void receivePacket(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED: // DISCONNECTION MESSAGE.
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: // CONNECTION MESSAGE.
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT: // TEXT TO BE PARSED BY AsciiMassageParser.
      Serial.printf("[%u] got Text: %s\n", num, payload);

      for ( int i = 0 ; i < lenght; i++ ) {
        if ( inbound.parse( payload[i] ) ) {
          if ( inbound.fullMatch("d") ) {
            digitalWrite(debugLedPin, inbound.nextInt() );
          } else if ( inbound.fullMatch("ms") ) {
            sendMs = inbound.nextInt();
          } else if ( inbound.fullMatch("a0") ) {
            sendAnalog = inbound.nextInt();
          } else {
            outbound.packEmpty("what?");
            sendPacket();
          }
        }
      }

      break;
  }

}

///////////
// SETUP //
///////////
void setup() {

  // INITIATE SERIAL COMMUNICATION.
  Serial.begin(57600);

  // SET PIN 13 (DEBUG LEG) AS OUTPUT.
  pinMode(debugLedPin, OUTPUT);

  // START THE ACCESS POINT WITH A BASE NAME OF "asciimassage".
  // THE LAST THREE HEX BYTES OF THE MAC ADDRESS WILL BE APPENDED
  // TO THE BASE NAME. SEE THE esp8266AP.h TAB FOR THE CODE.
  startWifiAp("asciimassage");

  webSocket.begin();
  webSocket.onEvent(receivePacket);

}


//////////
// LOOP //
//////////
void loop() {

  webSocket.loop();

  if ( sendMs && (millis() - lastTimeSentMs >= 1000) ) {
    lastTimeSentMs = millis();

    outbound.beginPacket("ms");
    outbound.addLong(millis());
    outbound.endPacket();
    sendPacket();

  }

  if ( sendAnalog && (millis() - lastTimeSentAnalogReading  >= 1000) ) {
    lastTimeSentAnalogReading = millis();
    outbound.packOneInt("a0", analogRead(0) );
    sendPacket();
  }

}




