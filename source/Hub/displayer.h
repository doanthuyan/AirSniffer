#ifndef DISPLAYER_H
#define DISPLAYER_H
#include "Sniffer_Data_Util.h"

#define HAS_LCD 1

void convertAirData(String dataStr,Environment * data);
void initLcd();
void showAirData(Environment *envData);

void showSmartConfig();
void askRestart();

#endif
