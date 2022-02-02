#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h> 
#include <DNSServer.h>

class _EzEsp8266 {
  


private:
/*
  const char* ssid;
  const char* password;
  IPAddress ip;
  IPAddress gateway;
  IPAddress subnet;
  
  void setup(const char* ssid, const char* password) {
      this->ssid = ssid;
      this->ssid = password;
   }

   void setup(const char* ssid, const char* password, IPAddress ip , IPAddress gateway, IPAddress subnet) {
      this->ip = ip;
      this->gateway = gateway;
      this->subnet = subnet;
      setup(ssid,pasword);
   }
*/
public:
  _EzEsp8266() {

    
  }


  void startAccessPoint(const char* ssid, const char* password) {
    //setup(ssid,password);
    WiFi.softAP(ssid, password);
    //this->ip = WiFi.softAPIP();
  }

   void startAccessPoint(const char* ssid, const char* password, IPAddress ip , IPAddress gateway, IPAddress subnet) {
      //setup(ssid, password, ip , ateway,subnet) ;
      WiFi.softAPConfig (ip, gateway, subnet) ;
      WiFi.softAP(ssid, password);
  }

  
  void connect(const char* ssid, const char* password) {
    //setup(ssid,password);
    WiFi.begin(ssid, password);
    
  }

   void connect(const char* ssid, const char* password, IPAddress ip , IPAddress gateway, IPAddress subnet) {
      //setup(ssid, password, ip , ateway,subnet) 
      WiFi.config(ip, gateway, subnet);
      WiFi.begin(ssid, password);
      
  }


  bool autoConnect(const char* fallback_ssid, const char* fallback_password) {
    WiFiManager wifiManager;
    return wifiManager.autoConnect(fallback_ssid,fallback_password);
  }

  void autoConnect(const char* fallback_ssid, const char* fallback_password, IPAddress ip , IPAddress gateway, IPAddress subnet) {
    
    WiFiManager wifiManager;
    wifiManager.setSTAStaticIPConfig(ip, gateway, subnet);
    wifiManager.autoConnect(fallback_ssid,fallback_password);
  }


  
};

_EzEsp8266 EzEsp8266;

