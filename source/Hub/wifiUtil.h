#ifndef _WIFI_UTIL_H
#define _WIFI_UTIL_H
#include "ESP8266WiFi.h"

#define RETRY_LIM 50
boolean testWifi(void);
void connectWifi();

void showWifiStatus();
#endif

