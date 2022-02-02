#include "EzEsp8266.h"
WiFiUDP udp;
#include "AsciiMassengerUdp.h"


AsciiMassengerUdp msg(&udp);

int remoteUdpPort = 4210;
int localUdpPort = 4210;

void setup() {
  // Initialize serial communication :  
  Serial.begin(57600);

  udp.begin(localUdpPort);

  EzEsp8266.startAccessPoint("esp8266","password");

  /*
     if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
   */
 
}


  void loop()
{

    if ( msg.parsePacket() ) {

     
 
  if ( msg.fullMatch("yo") ) {
      msg.beginPacket("received yo",udp.remoteIP(), remoteUdpPort);
      msg.addFloat(12.3);
      msg.addFloat(320);
      msg.endPacket();
  }
    // send back a reply, to the IP address and port we got the packet from
      msg.beginPacket("test",udp.remoteIP(), remoteUdpPort);
      msg.addFloat(12.3);
      msg.endPacket();
   
      
    }
   
  

}


