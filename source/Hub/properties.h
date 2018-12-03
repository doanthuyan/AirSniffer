#ifndef _PROPERTIES_H
#define _PROPERTIES_H
#include <LiquidCrystal_I2C.h>
//#include <WiFiClientSecure.h>
//#include <ESP8266WebServer.h>
#include <Sniffer_Rest_Util.h>

#define TEMP_SENSOR "DHT11"
#define PM_SENSOR "Nova fitness"

#define updateInterval  30 * 1000 

extern LiquidCrystal_I2C lcd;


#define CONFIG_BTN 2
#define ERR_PIN 14
#define OK_PIN 15
#define RF_RX 12
#define RF_TX 13
#define REF_PIN 16
#endif
