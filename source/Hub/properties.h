#ifndef _PROPERTIES_H
#define _PROPERTIES_H
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>


struct Environment{
  float novaPm25;
  float novaPm10;
  float temperature;
  float humidity;

};

extern Environment envData;
extern LiquidCrystal_I2C lcd;

const long interval = 20000; 

//#define serverAddress "api.thingspeak.com"
//#define writeAPIKey  "7X8TBTEOMUESVGPQ"
#define serverAddress "crowdsniffing.org"
#define url "/api/pollutantvalues"
#define sender "1A4hRM7ti7"
#define macStr "FF-A2-DA-00-3F-00"

#define latitude "10.798995"
#define longitude "106.68691"

#define TEMP_SENSOR "DHT11"
#define PM_SENSOR "Nova fitness"

#define updateInterval  30 * 1000 
#define httpPort 80
#define httpsPort 443


#define smartConfigSSID "SNIFFER"


extern IPAddress myIP;
/* Set these to your desired credentials. */
#define CONFIG_MODE 1
#define NORM_MODE 0

typedef struct {
   uint8_t mode;
   char ssid[36];
   char pwd[50];
   //char code[10];
} HubConfig;

extern ESP8266WebServer server;
extern HubConfig hubConfig;
extern bool wifiStatus;

#define CONFIG_BTN 2
#define WIFI_ERR_PIN 14
#define RF_RX 12
#define RF_TX 13
#endif

