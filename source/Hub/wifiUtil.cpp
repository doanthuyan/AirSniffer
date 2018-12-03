#include "wifiUtil.h"
#include "properties.h"
//#include <ESP8266WiFi.h>
#include "smartConfig.h"
#include "constants.h"
#include "displayer.h"
#include <EEPROM.h>

/* Set these to your desired credentials. */

void connectWifi(){
  
  //if(esid.length()>1){
      // test esid 
      //strcpy(hubConfig.ssid,esid.c_str());
      //strcpy(hubConfig.pwd,epass.c_str());
      WiFi.mode(WIFI_STA);
      WiFi.begin(hubConfig.ssid, hubConfig.pwd);
      if(testWifi()){
        Serial.println("Wifi connected, running in normal mode");
        hubConfig.mode = NORM_MODE;
        wifiStatus = true;
        //lcd.setCursor(0,0);
        //lcd.print("Wifi connected!");
      }else{
        hubConfig.mode = NORM_MODE;
        wifiStatus = false;
        //lcd.setCursor(0,0);
        Serial.println("Wifi not connected!");
      }
      
  //}else{
  //  hubConfig.mode = CONFIG_MODE;
  //  setupAP(); 
  //  showSmartConfig(false);
  //}
  Serial.print("Mode: " );
  Serial.println(hubConfig.mode);
  showWifiStatus();
}

boolean testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");  
  while ( c < RETRY_LIM ) {
    if (WiFi.status() == WL_CONNECTED) { return true; } 
    delay(1000);
    Serial.print(WiFi.status());    
    c++;
  }
  Serial.println("Connect timed out, no internet");
  return false;
} 


void showWifiStatus(){
  if(wifiStatus){
    digitalWrite(WIFI_ERR_PIN,LOW);
  }else{
    digitalWrite(WIFI_ERR_PIN,HIGH);
  }
}
