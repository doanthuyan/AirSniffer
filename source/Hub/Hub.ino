#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "displayer.h"
#include "properties.h"
#include <Sniffer_Smart_Config.h>
#include <Sniffer_Wifi_Util.h>
#include <Sniffer_Dust_Sensor.h>
#include <Sniffer_Data_Util.h>
#include <Sniffer_Rest_Util.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

//LiquidCrystal_I2C lcd(0x3f,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(RF_RX, RF_TX);
Environment envData;
HubConfig hubConfig;


String theMessage = "";
char data[50]= {0};

int i=0;
int btnVoltage, refVoltage;

void highInterrupt();
void showStatus(bool ok);

void setup(){
  //ESP.eraseConfig();
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println("Hub started");
  pinMode(CONFIG_BTN, INPUT_PULLUP);
  pinMode(ERR_PIN, OUTPUT);
  pinMode(REF_PIN, OUTPUT);
  pinMode(OK_PIN, OUTPUT);
  
  initLcd();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sniffer's Hub OK");
  
  initSnifferConfig(&hubConfig);
  attachInterrupt(CONFIG_BTN, highInterrupt, FALLING);
  if(isConfigMode(&hubConfig)){
    setupAP(&hubConfig);
    showSmartConfig();
  }else{
    connectWifi(&hubConfig);

    lcd.setCursor(0,1);
    if(WiFi.status() != WL_CONNECTED){
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
  
  
}

void loop(){
  //Serial.print("Mode: " );
  //Serial.println(hubConfig.mode);
  
  if(needRestart()){
    //Serial.println("need restart");
    
    #if defined(HAS_LCD)
      Serial.println("Please restart your Sniffer");
      askRestart();
      delay(1000);
    #else
      ESP.restart();
    #endif
  }else{  
    if(isConfigMode(&hubConfig)){
      //Serial.println("In config mode");
      handleSmartConfigClient();
      delay(6000);
    }else{
      //Serial.println("In normal mode");
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
        convertAirData(theMessage, &envData);
        showAirData(&envData);
        
        if(WiFi.status() != WL_CONNECTED){
          connectWifi(&hubConfig);
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
          showStatus(ok);
        }else{
          showStatus(false);
        }
        Serial.println();
      }
      // for LCD 2 lines only 
      else{
        //Serial.println("for LCD 2 lines only ");
         if(isValidAirData(envData)){
           showStatus(true);
           showAirData(&envData);
           delay(5000);
         }
      }
    }
  }
}


void highInterrupt(){
  btnVoltage = analogRead(CONFIG_BTN);
  refVoltage = analogRead(REF_PIN);
  if(btnVoltage == refVoltage){
  
    prepareSmartConfig(&hubConfig);
    ESP.restart();
  }
}
void showStatus(bool ok){
  if(ok){
    digitalWrite(OK_PIN,HIGH);
    digitalWrite(ERR_PIN,LOW);
  }else{
    digitalWrite(OK_PIN,LOW);
    digitalWrite(ERR_PIN,HIGH);
  }
}

