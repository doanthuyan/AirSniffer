#ifndef _DATAUTIL_H
#define _DATAUTIL_H
#include <ESP8266WiFi.h>
void convertAirData(String dataStr);
void formatAAVNData(char * dataStr);
bool isValidAirData();
#endif
