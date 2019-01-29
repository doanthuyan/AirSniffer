#ifndef _DATAUTIL_H
#define _DATAUTIL_H
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <Sniffer_Rest_Property.h>

struct Environment{
  float novaPm25;
  float novaPm10;
  float temperature;
  float humidity;

};

void convertAirData(String dataStr);
void formatAAVNData(char * dataStr, Environment envirData, RestProperty restProperty);
bool isValidAirData(Environment envirData);
void printData(Environment envirData);

#endif
