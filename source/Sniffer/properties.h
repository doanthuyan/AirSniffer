#ifndef _PROPERTIES_H
#define _PROPERTIES_H
#include "Arduino.h"
#include <SoftwareSerial.h>


#define STRING_SIZE 80
#define SAMPLES 10
#define SLEEP_PERIOD 15 //15 minutes sleep
#define SENSOR_ERR 3
#define LED_SIGNAL 4
#define NOVA_RX 8
#define NOVA_TX 7

struct Environment{
  float novaPm25;
  float novaPm10;
  float temperature;
  float humidity;
};

extern Environment envData;


#endif

