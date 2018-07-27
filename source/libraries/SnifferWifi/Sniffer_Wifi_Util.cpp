#include "Sniffer_Wifi_Util.h"
#include <EEPROM.h>

//private functions
boolean testWifi(void);

/* Set these to your desired credentials. */
void connectWifi(HubConfig* smartConfig){
  
      WiFi.mode(WIFI_STA);
      WiFi.begin(smartConfig->ssid, smartConfig->pwd);
      if(testWifi()){
        Serial.println("Wifi connected, running in normal mode");
      }else{
        Serial.println("Wifi not connected!");
      }
      
  Serial.print("Mode: " );
  Serial.println(smartConfig->mode);
  
}

boolean testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");  
  while ( c < RETRY_LIM ) {
    if (WiFi.status() == WL_CONNECTED) { 
      break; 
      } 
    delay(1000);
	Serial.print("Wifi status: ");
    Serial.println(WiFi.status());    
    c++;
  }
  if (WiFi.status() == WL_CONNECTED) { 
    return true; 
  } else{
    Serial.println("Connect timed out, no internet");
    return false;
  }
  
} 

