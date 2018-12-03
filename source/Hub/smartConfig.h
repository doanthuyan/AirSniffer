#ifndef _SMARTCONFIG_H
#define _SMARTCONFIG_H

void setupAP(void);

void handleRoot();
void storeConfig();
void loadConfig();
void handlePwd();
void wifiConfigResult();

void launchWeb(void);

//just for testing
void clearStoredConfig();
#endif
