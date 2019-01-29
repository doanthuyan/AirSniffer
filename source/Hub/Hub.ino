#include <EEPROM.h>
#include <SPI.h>
#include <dht.h>
#include <Sniffer_Smart_Config.h>
#include <Sniffer_Wifi_Util.h>
#include <Sniffer_Dust_Sensor.h>
#include <Sniffer_Data_Util.h>
#include <Sniffer_Rest_Util.h>
#include "properties.h"
#include "displayer.h"

LiquidCrystal_I2C lcd(0x3f,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(RF_RX, RF_TX);
Environment envData;

HubConfig hubConfig;
bool wifiStatus = false;


String theMessage = "";
char data[50]= {0};

int i=0;
int blink_type = 1; //0: no blink; 1: blink; 2: flash twice; 3: flash three

void highInterrupt();
void showStatus(bool ok);
void convertAirData(String dataStr);

void setup(){
  //ESP.eraseConfig();
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println("Hub started");
  pinMode(CONFIG_BTN, INPUT_PULLUP);
  pinMode(ERR_PIN, OUTPUT);
  digitalWrite(ERR_PIN,HIGH);
  
  initLcd();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sniffer's Hub OK");

  //EEPROM.begin(512);
  //delay(10);
  initSnifferConfig(&hubConfig);
  delay(50);
  
  
  if(isConfigMode(&hubConfig)){
    setupAP(&hubConfig);
    showSmartConfig();
  }else{
    connectWifi(&hubConfig, ERR_PIN);
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
  if(needRestart()){
      ESP.restart();
    }else{
      if(isConfigMode(&hubConfig)){
        showSmartConfig();
        handleSmartConfigClient();
        delay (600);
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
            connectWifi(&hubConfig, ERR_PIN);
          }
          
          if(WiFi.status() == WL_CONNECTED){
            RestProperty restProperty;
            restProperty.sender_pro= hubConfig.code;
            restProperty.latitude_pro= hubConfig.latitude;
            restProperty.longitude_pro= hubConfig.longitude;
            restProperty.TEMP_SENSOR_pro= TEMP_SENSOR;
            restProperty.PM_SENSOR_pro= PM_SENSOR;
            restProperty.mac_str_pro = hubConfig.macStr;
            bool ok = saveData(envData,restProperty);
            Serial.print("Save data:");
            Serial.println(ok);
            WiFi.disconnect();
            if (ok) { 
              blink_type = 0; //everything ok, no blink
              showStatus(ok);
              
            } else {
              blink_type = 2;
            }
          } else{
//            showStatus(false);
            blink_type = 1;
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
}
void showStatus(bool ok){
  if(ok){
//    digitalWrite(OK_PIN,HIGH);
    digitalWrite(ERR_PIN,HIGH);
  }else{
//    digitalWrite(OK_PIN,LOW);
    digitalWrite(ERR_PIN,LOW);
  }
}


void highInterrupt(){
  
    WiFi.disconnect();
    prepareSmartConfig(&hubConfig);
    //delay(5000);
    ESP.restart();
}
void convertAirData(String dataStr){
  int index = 0, nextIndex;
  String subStr;

  nextIndex = dataStr.indexOf(',',index);
  if(nextIndex > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.novaPm25 = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.novaPm10 = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.temperature = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.humidity = subStr.toFloat();
  }
}
