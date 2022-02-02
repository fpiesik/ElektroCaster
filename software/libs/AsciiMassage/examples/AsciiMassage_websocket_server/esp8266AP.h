// INCLUDE ESP8266 WIFI
#include <ESP8266WiFi.h>


// APPEND THE LAST THREE BYTES OF THE MAC ADDRESS (HEX'd)TO THE ACCESS POINT BASE NAME.
void startWifiAp(char * accessPointBaseName ) {

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String threeLastHexBytes = String(mac[WL_MAC_ADDR_LENGTH - 3], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  threeLastHexBytes.toLowerCase();
  String accessPointNameString = String(accessPointBaseName) + threeLastHexBytes;
  // CONVERT THE ACCESS POINT NAME STRING TO A CHAR *
  char accessPointName[accessPointNameString.length() + 1]; // CREATE THE CHAR * FOR THE FINAL ACCESS POINT NAME.
  memset(accessPointName, 0, accessPointNameString.length() + 1); // FILL WITH ZEROS.
  for (int i = 0; i < accessPointNameString.length(); i++) accessPointName[i] = accessPointNameString.charAt(i); // COPY.

  // SETUP ACCESS POINT.
  WiFi.mode(WIFI_AP);
  WiFi.softAP(accessPointName); // WARNING: NO PASSWORD.
}

