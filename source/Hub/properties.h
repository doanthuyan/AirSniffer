#ifndef _PROPERTIES_H
#define _PROPERTIES_H
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Sniffer_Rest_Util.h>
#include <Sniffer_Dust_Sensor.h>

extern Environment envData;
extern LiquidCrystal_I2C lcd;

const long interval = 20000; 



#define TEMP_SENSOR "DHT11"
#define PM_SENSOR "Nova fitness"

#define updateInterval  30 * 1000 

/* Set these to your desired credentials. */
#define CONFIG_MODE 1
#define NORM_MODE 0


extern dht DHT;

#define CONFIG_BTN 2
#define ERR_PIN 14
#define RF_RX 12
#define RF_TX 13
#endif
