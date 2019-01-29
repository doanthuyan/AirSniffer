#ifndef _REST_PROPERTY_H
#define _REST_PROPERTY_H
#include <SPI.h>

#define serverAddress "crowdsniffing.org"
#define url "/api/pollutantvalues"

#define httpsPort 443


struct RestProperty{
  String sender_pro;
  String latitude_pro;
  String longitude_pro;
  String TEMP_SENSOR_pro;
  String PM_SENSOR_pro;
  String mac_str_pro;
};


#endif
