#ifndef _WIFI_UTIL_H
#define _WIFI_UTIL_H
#include <ESP8266WiFi.h>
#include "Sniffer_Smart_Config.h"

#define RETRY_LIM 50


void connectWifi(HubConfig* smartConfig, int ERR_PIN);


#endif
