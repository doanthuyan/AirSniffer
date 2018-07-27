#include "Sniffer_Smart_Config.h"
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
//Sniffer state
#define CONFIG_MODE 1
#define NORM_MODE 0
#define ONBOARDED -1

//private data
String configPage;
ESP8266WebServer server(httpPort);
int snifferState;
HubConfig * _oldConfig;

//private functions
void handleRoot();
void handlePwd();
void wifiConfigResult();
void launchWeb(void);
void clearStoredConfig(void);
void clearStoredWifi(void);

///////////////////

void setupAP(HubConfig* oldConfig) {
  _oldConfig = oldConfig;
  configPage.reserve(3000);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  configPage = "";
  configPage.concat("<html lang=\"en-us\"><head><title>Sniffer configuration</title>");
  configPage.concat("<style>h1 {color: CornflowerBlue ;} h2 {color: red;} .city {float: left;margin: 5px;padding: 15px;} ");
  configPage.concat(".inline {display: inline;} ");
  configPage.concat(".link-button {background: none; border: none; color: blue; text-decoration: underline; cursor: pointer; font-size: 1em; font-family: serif;} ");
  configPage.concat(".link-button:focus {outline: none;} ");
  configPage.concat(".link-button:active { color:red;} ");
  configPage.concat("</style></head><body><h1>Sniffer Configuration</h1>");
  configPage.concat("<div class=\"city\"><h2>Choose a WIFI network for your Sniffer</h2><ol>");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0){
    Serial.println("no networks found");
    configPage.concat("</ol>no networks found</div></body></html>");
  }else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
      //configPage.concat("<li><a href=\"/inputPwd/"+WiFi.SSID(i)+"\">"+WiFi.SSID(i)+"</a></li>");
      configPage.concat("<li><form method=\"post\" action=\"/inputPwd\" class=\"inline\">");
      configPage.concat("<input type=\"hidden\" name=\"ssid\" value=\""+ WiFi.SSID(i)+ "\">");
      configPage.concat("<button type=\"submit\" name=\"submit_param\" value=\"submit_value\" class=\"link-button\">");
      configPage.concat(WiFi.SSID(i)+"</button></form></li>");
     }
     configPage.concat("</ol></div></body></html>");
  }
  delay(100);
  WiFi.softAP(smartConfigSSID);
  Serial.println("softap");
  Serial.println("");
  launchWeb();
  Serial.println("over");
}


void launchWeb(void) {
    Serial.println("");
    Serial.println("Webserver started!");
    Serial.println(WiFi.softAPIP());

    // Start the server
    server.on("/", handleRoot);
    server.on("/inputPwd",handlePwd);
    server.on("/wifi_config/result",wifiConfigResult);
    
    server.begin();
    Serial.println("Server started");   

}
void wifiConfigResult(){
	HubConfig smartConfig;
    String htmlStr="<h1>Configuration stored. Restarting!</h1>";
    String ssid, pwd, code, latitude, longitude, macStr;
    Serial.println("Args: ");
    for(int i = 0; i < server.args(); i++){
      Serial.print(server.argName(i));
      Serial.print(": ");
      Serial.println(server.arg(i));
    }
    Serial.println();
    Serial.println("Headers: ");
    for(int i = 0; i < server.headers(); i++){
      Serial.print(server.headerName(i));
      Serial.print(": ");
      Serial.println(server.header(i));
    }
    Serial.println();
    ssid = server.arg("ssid");
    pwd = server.arg("pwd");
	code = server.arg("code");
    latitude = server.arg("lat");
	longitude = server.arg("long");
    macStr = server.arg("mac");
	
    strcpy(smartConfig.ssid,ssid.c_str());
    strcpy(smartConfig.pwd,pwd.c_str());
	strcpy(smartConfig.code,code.c_str());
    strcpy(smartConfig.latitude,latitude.c_str());
	strcpy(smartConfig.longitude,longitude.c_str());
    strcpy(smartConfig.macStr,macStr.c_str());
    
    server.send(200, "text/html", htmlStr);
    smartConfig.mode = NORM_MODE;
    storeConfig(&smartConfig);
	
	snifferState = ONBOARDED;
		
}
void handlePwd(){
 
  Serial.println("Args: ");
  for(int i = 0; i < server.args(); i++){
    Serial.print(server.argName(i));
    Serial.print(": ");
    Serial.println(server.arg(i));
  }
  Serial.println();
  String decodedStr = server.arg("ssid");
  
  
  String htmlPage ="<html><body><h2>Enter Sniffer Config</h2>";

	htmlPage.concat("<form method=\"POST\" action=\"/wifi_config/result\">");
	htmlPage.concat("<table><tr><td>Wifi</td><td colspan=2><input readonly name=\"ssid\" id=\"ssid\" value=\"" + decodedStr +"\"\\></td></tr>");
	htmlPage.concat("<tr><td>Password</td><td colspan=2><input type=\"text\" name=\"pwd\" id=\"pwd\" \"\\></td></tr>");
	htmlPage.concat("<tr><td colspan=3><hr></td></tr>");
	decodedStr="";
	decodedStr.concat (_oldConfig->code);
	htmlPage.concat("<tr><td>Code</td><td colspan=2><input type=\"text\" name=\"code\" id=\"code\" value=\"" + decodedStr +"\"\\></td></tr>");
	htmlPage.concat("<tr><td rowspan=2><b>GPS</b></td>");
	decodedStr="";
	decodedStr.concat (_oldConfig->latitude);
	htmlPage.concat("<td>Latitude</td><td><input type=\"text\" name=\"lat\" id=\"lat\" value=\"" + decodedStr +"\"\\></td></tr>");
	decodedStr="";
	decodedStr.concat (_oldConfig->longitude);
	htmlPage.concat("<tr><td>Longitude</td><td><input type=\"text\" name=\"long\" id=\"long\" value=\"" + decodedStr +"\"\\></td></tr>");
	decodedStr="";
	decodedStr.concat (_oldConfig->macStr);
	htmlPage.concat("<tr><td>MAC</td><td colspan=2><input type=\"text\" name=\"mac\" id=\"mac\" value=\"" + decodedStr +"\"\\></td></tr>");
	htmlPage.concat("<tr><td></td><td><input type=\"submit\" value=\"Connect\"\\></td></tr>");
	htmlPage.concat("</table></form></body></html>");
  
  
  server.send(200, "text/html",htmlPage);
}
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  //server.send(200, "text/html", "<h1>You are connected</h1>");
  //Serial.println("Sending: ");
  //Serial.println(configPage);
  server.send(200, "text/html",configPage);
}

void loadConfig(HubConfig* smartConfig){
  //read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");
  smartConfig->mode = (uint8_t)(EEPROM.read(0));
  Serial.print("Mode: ");
  Serial.println(smartConfig->mode);
  int usedByteCount = sizeof(smartConfig->mode);

  String esid="",epass = "",ecode ="",elatitude="", elongitude="", emac="";
  for (int i = usedByteCount; i < usedByteCount + sizeof(smartConfig->ssid); ++i)
    {
      if(EEPROM.read(i) != 0){
        esid += char(EEPROM.read(i));
      }
    }
  esid.trim();
  Serial.print("eSSID: ");
  Serial.print(esid);Serial.println("\\END");  
  Serial.print("esid length: ");
  Serial.print(esid.length());
  Serial.println("Reading EEPROM pass");
  //read Password
  usedByteCount += sizeof(smartConfig->ssid);
  for (int i = usedByteCount; i <usedByteCount + sizeof(smartConfig->pwd); ++i)
    {
      epass += char(EEPROM.read(i));
    }
  Serial.print("ePASS: ");
  Serial.print(epass);  
  Serial.println("\\END");  
  esid.trim();
  //read Sniffer Code
  usedByteCount += sizeof(smartConfig->pwd);
  for (int i = usedByteCount; i <usedByteCount + sizeof(smartConfig->code); ++i)
    {
		if(isAscii(char(EEPROM.read(i))))
			ecode += char(EEPROM.read(i));
    }
  Serial.print("eCODE: ");
  Serial.print(ecode);  
  Serial.println("\\END");  
  ecode.trim();
  //read Sniffer Latitude
  usedByteCount += sizeof(smartConfig->code);
  for (int i = usedByteCount; i <usedByteCount + sizeof(smartConfig->latitude); ++i)
    {
		if(isAscii(char(EEPROM.read(i))))
			elatitude += char(EEPROM.read(i));
    }
  Serial.print("eLAT: ");
  Serial.print(elatitude);  
  Serial.println("\\END");  
  elatitude.trim();
  //read Sniffer Longitude
  usedByteCount += sizeof(smartConfig->latitude);
  for (int i = usedByteCount; i <usedByteCount + sizeof(smartConfig->longitude); ++i)
    {
		if(isAscii(char(EEPROM.read(i))))
			elongitude += char(EEPROM.read(i));
    }
  Serial.print("eLONG: ");
  Serial.print(elongitude);  
  Serial.println("\\END");  
  elongitude.trim();
  //read Sniffer MAC
  usedByteCount += sizeof(smartConfig->longitude);
  for (int i = usedByteCount; i <usedByteCount + sizeof(smartConfig->macStr); ++i)
    {
		if(isAscii(char(EEPROM.read(i))))
			emac += char(EEPROM.read(i));
    }
  Serial.print("eMAC: ");
  Serial.print(emac);  
  Serial.println("\\END");  
  emac.trim();
  
  strcpy(smartConfig->ssid,esid.c_str());
  strcpy(smartConfig->pwd,epass.c_str());
  strcpy(smartConfig->code,ecode.c_str());
  strcpy(smartConfig->latitude,elatitude.c_str());
  strcpy(smartConfig->longitude,elongitude.c_str());
  strcpy(smartConfig->macStr,emac.c_str());

  printConfig(smartConfig);
}
void storeConfig(HubConfig* smartConfig){
  printConfig(smartConfig);
  Serial.println("clearing eeprom");
  
  String qsid = "",qpass="", qcode ="",qlatitude="", qlongitude="", qmac="";
  
  
  for (int i = 0; i < sizeof(smartConfig); ++i) { EEPROM.write(i, '\0'); }
  EEPROM.commit();
  delay(100);
  EEPROM.write(0, smartConfig->mode);
  int usedByteCount = sizeof(smartConfig->mode);
  qsid.concat(smartConfig->ssid);
  Serial.println(qsid);
  Serial.print("");
  
  qpass.concat(smartConfig->pwd);
  Serial.print(qpass);
  Serial.println("");
  qcode.concat(smartConfig->code);
  Serial.print(qcode);
  Serial.println("");
  qlatitude.concat(smartConfig->latitude);
  Serial.print(qlatitude);
  Serial.println("");
  qlongitude.concat(smartConfig->longitude);
  Serial.print(qlongitude);
  Serial.println("");
  qmac.concat(smartConfig->macStr);
  Serial.print(qmac);
  Serial.println("");
  
  Serial.println("writing eeprom ssid:");
  for (int i = 0; i < qsid.length(); ++i)
    {
      EEPROM.write(usedByteCount+i, qsid[i]);
      Serial.print("Wrote: ");
      Serial.println(qsid[i]); 
    }

  usedByteCount += sizeof(smartConfig->ssid);
  Serial.println("writing eeprom pass:"); 
  for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(usedByteCount+i, qpass[i]);
      Serial.print("Wrote: ");
      Serial.println(qpass[i]); 
    }    
  usedByteCount += sizeof(smartConfig->pwd);
  Serial.println("writing eeprom code:"); 
  for (int i = 0; i < qcode.length(); ++i)
    {
      EEPROM.write(usedByteCount+i, qcode[i]);
      Serial.print("Wrote: ");
      Serial.println(qcode[i]); 
    }
  
  usedByteCount += sizeof(smartConfig->code);
  Serial.println("writing eeprom lat:"); 
  int j;
  for (j = 0; j < qlatitude.length(); ++j)
    {
      EEPROM.write(usedByteCount+j, qlatitude[j]);
      Serial.print("Wrote: ");
      Serial.println(qlatitude[j]); 
    }
	EEPROM.write(usedByteCount+j, '\0');
  usedByteCount += sizeof(smartConfig->latitude);
  Serial.println("writing eeprom long:"); 
  for (int i = 0; i < qlongitude.length(); ++i)
    {
      EEPROM.write(usedByteCount+i, qlongitude[i]);
      Serial.print("Wrote: ");
      Serial.println(qlongitude[i]); 
    }
  usedByteCount += sizeof(smartConfig->longitude);
  Serial.println("writing eeprom mac:"); 
  for (int i = 0; i < qmac.length(); ++i)
    {
      EEPROM.write(usedByteCount+i, qmac[i]);
      Serial.print("Wrote: ");
      Serial.println(qmac[i]); 
    }
  EEPROM.commit();
}
void clearStoredWifi(){
	HubConfig * dummy;
  Serial.println("\nclearing wifi config only");
        for (int i = 0; i < sizeof(dummy->mode)+sizeof(dummy->ssid)+sizeof(dummy->pwd); ++i) { 
			EEPROM.write(i, 0); 
		}

  EEPROM.commit();
}
void clearStoredConfig(){
	HubConfig * dummy;
  Serial.println("\nclearing wifi config only");
        for (int i = 0; i < sizeof(HubConfig); ++i) { 
			EEPROM.write(i, 0); 
		}

  EEPROM.commit();
}
bool isConfigMode(HubConfig* smartConfig){
	return smartConfig->mode == CONFIG_MODE;
}
void handleSmartConfigClient(){
	server.handleClient();
}
void initSnifferConfig(HubConfig* smartConfig){
	
  EEPROM.begin(1024);
  delay(10);
  loadConfig(smartConfig);
  snifferState = smartConfig->mode;
  delay(50);
  
}
void prepareSmartConfig(HubConfig* smartConfig){
	smartConfig->mode = CONFIG_MODE;
	smartConfig->ssid[0] = '\0';
	smartConfig->pwd[0] = '\0';
	//smartConfig->code[0] = '\0';
	//smartConfig->latitude[0] = '\0';
	//smartConfig->longitude[0] = '\0';
	//smartConfig->macStr[0] = '\0';
	//clearStoredConfig();
    //hubConfig.mode = NORM_MODE;
	clearStoredWifi();
    storeConfig(smartConfig);
    
}
void printConfig(HubConfig* smartConfig){
	Serial.print("SSID: "); Serial.println(smartConfig->ssid);
	Serial.print("PWD: "); Serial.println(smartConfig->pwd);
	Serial.print("CODE: "); Serial.println(smartConfig->code);
	Serial.print("LAT: "); Serial.println(smartConfig->latitude);
	Serial.print("LONG: "); Serial.println(smartConfig->longitude);
	Serial.print("MAC: "); Serial.println(smartConfig->macStr);
}
bool needRestart(){
	return snifferState == ONBOARDED;
}