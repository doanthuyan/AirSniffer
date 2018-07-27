#include <SPI.h>
#include <dht.h>
#include <Sniffer_Smart_Config.h>
#include <Sniffer_Wifi_Util.h>
#include <Sniffer_Dust_Sensor.h>
#include <Sniffer_Data_Util.h>
#include <Sniffer_Rest_Util.h>
//for portable IDE
/*#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>*/

#include "properties.h"

SnifferDustSensor dustSensor;
Environment envData;

HubConfig hubConfig;
dht DHT;

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
  //pinMode(ERR_PIN, OUTPUT);
  pinMode(REF_PIN, OUTPUT);
  //pinMode(OK_PIN, OUTPUT);
 

  initSnifferConfig(&hubConfig);
  if(isConfigMode(&hubConfig)){
    setupAP(&hubConfig);
    
  }else{
    //connectWifi(&hubConfig);

    //start ok
    
    dustSensor.begin(NOVA_RX,NOVA_TX);
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
  }
  
  attachInterrupt(CONFIG_BTN, highInterrupt, FALLING);
}

void loop(){
  //Serial.print("Mode: " );
  //Serial.println(hubConfig.mode);
  if(needRestart()){
    ESP.restart();
  }else{
    if(isConfigMode(&hubConfig)){
      
      handleSmartConfigClient();
      delay(6000);
    }else{
      
      readSnifferData();
      if (isValidAirData(envData)) {
        showStatus(true);
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
      }else{
        showStatus(false);
      }
      delay(600000);
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
void readSnifferData()
{
    
    //delay(5000);
    //read dust sensor value
    Serial.println("Reading PM Data");
    bool ok = dustSensor.readDustData();
    Serial.println("Ending read PM Data");
    //if(ok){
      envData.novaPm25 = dustSensor.getPM25();
      envData.novaPm10 = dustSensor.getPM10();
      
      Serial.print ("PM10:");
      Serial.println(dustSensor.getPM10());
      Serial.print ("PM2.5:");
      Serial.println(dustSensor.getPM25());
    
    //}
     
    //read 
    readTemperatureAndHumidity();
    while(envData.temperature <= 0 || envData.humidity <= 0){
      Serial.println("DHT11 sensor doesn't work properly");
      readTemperatureAndHumidity();  
      delay(5000);
    }
    printData(envData);
    delay(1000);
    Serial.println("Done reading data");
       
   
}


//read temperature and humidity from DHT11
void readTemperatureAndHumidity(){
  
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }


  envData.temperature = DHT.temperature;
  envData.humidity = DHT.humidity;
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  
  }
void showStatus(bool ok){
  if(ok){
    //digitalWrite(OK_PIN,HIGH);
    //digitalWrite(ERR_PIN,LOW);
  }else{
    //digitalWrite(OK_PIN,LOW);
    //digitalWrite(ERR_PIN,HIGH);
  }
}

