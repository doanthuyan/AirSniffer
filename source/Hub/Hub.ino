#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <ArduinoJson.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "displayer.h"
#include "properties.h"
#include "wifiUtil.h"
#include "dataUtil.h"
#include "restUtil.h"
#include "constants.h"
#include "smartConfig.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x3f,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(RF_RX, RF_TX);
Environment envData;
ESP8266WebServer server(httpPort);
HubConfig hubConfig;
bool wifiStatus = false;


String theMessage = "";
char data[STRING_SIZE]= {0};

int i=0;

void highInterrupt();


void setup(){
  //ESP.eraseConfig();
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println("Hub started");
  pinMode(CONFIG_BTN, INPUT_PULLUP);
  pinMode(WIFI_ERR_PIN, OUTPUT);
  digitalWrite(WIFI_ERR_PIN,HIGH);
  
  initLcd();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sniffer's Hub OK");

  EEPROM.begin(512);
  delay(10);
  loadConfig();
  delay(50);
  
  
  if(hubConfig.mode == CONFIG_MODE){
    setupAP();
    showSmartConfig();
  }else{
    connectWifi();
    lcd.setCursor(0,1);
    if(wifiStatus){
      lcd.print("Wifi OK");
    }else{
      lcd.print("Wifi not connect");
    }
    //start ok
    mySerial.begin(9600);
    Serial.println("Receiver ready");
    lcd.setCursor(0,2);
    lcd.print("Receiver ready!");
    delay (50);
    lcd.setCursor(0,3);
    lcd.print("Wait for Sniffer!");
    
  }
  
  attachInterrupt(CONFIG_BTN, highInterrupt, RISING);
}

void loop(){
  //Serial.print("Mode: " );
  //Serial.println(hubConfig.mode);

  if(hubConfig.mode == CONFIG_MODE){
    showSmartConfig();
    server.handleClient();
  }else{
    theMessage= ""; 
    if (mySerial.available() > 0) {
      Serial.println("Get value from sniffer ");
      delay(20);
      while (mySerial.available() > 0) {
        
        char ch = mySerial.read(); //read
        Serial.write(ch); //display on monitor
        theMessage.concat(ch);
        delay(30);
      }
      Serial.println();
      Serial.println(theMessage);
      convertAirData(theMessage);
      showAirData();
      
      if(WiFi.status() != WL_CONNECTED){
        connectWifi();
      }
      
      if(wifiStatus){
        saveData();
      }
      Serial.println();
    }
    // for LCD 2 lines only 
    /*else{
       if(isValidAirData()){
         showAirData();
         delay(5000);
       }
    }*/
  }
  
}


void highInterrupt(){
  
  hubConfig.mode = CONFIG_MODE;
  storeConfig();
  ESP.restart();
}



