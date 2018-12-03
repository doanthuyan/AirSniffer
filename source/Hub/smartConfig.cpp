#include "smartConfig.h"
#include "properties.h"
//#include <WiFiClient.h>
#include <EEPROM.h>
String st;
String configPage;

void setupAP(void) {
  
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
  configPage.concat("</style>");
  configPage.concat("</head><body>");
  configPage.concat("<h1>Sniffer Configuration</h1>");
  configPage.concat("<div class=\"city\"><h2>Choose a WIFI network for your Sniffer</h2>");
  configPage.concat("<ol>");
  //configPage.concat("<form method=\"POST\" action=\"/inputPwd\"><ol>");
  //configPage.concat("<tr><td><b>SSID</b></td><td><input type=\"text\" id=\"ssid\" name=\"ssid\" autofocus required></td></tr>");
  //configPage.concat("<tr><td><b>Password</b></td><td><input type=\"text\" name=\"pwd\" id=\"pwd\"></td></tr>");
  //configPage.concat("<tr><td></td><td><input type=\"submit\" value=\"Save\"></input></td></tr>");

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
  Serial.println(""); 
  st = "<ul>";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st +=i + 1;
      st += ": ";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</li>";
    }
  st += "</ul>";
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
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.softAPIP());

    // Start the server
    server.on("/", handleRoot);
    server.on("/inputPwd",handlePwd);
    server.on("/wifi_config/result",wifiConfigResult);
    
    server.begin();
    Serial.println("Server started");   

}
void wifiConfigResult(){
    String htmlStr="<h1>Configuration stored. Hub restarting!</h1>";
    String ssid, pwd;
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
    strcpy(hubConfig.ssid,ssid.c_str());
    strcpy(hubConfig.pwd,pwd.c_str());
    
    server.send(200, "text/html", htmlStr);
    hubConfig.mode = NORM_MODE;
    storeConfig();
    //connectWifi();
    ESP.restart();
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
  
  String htmlPage = "<html><body><h2>Enter WIFI password for <b>" +decodedStr +"</b></h2>";
  htmlPage.concat("<form method=\"POST\" action=\"/wifi_config/result\">");
  htmlPage.concat("<input type=\"hidden\" name=\"ssid\" id=\"ssid\" value=\"" +decodedStr+"\"\\>");
  htmlPage.concat("<input type=\"text\" name=\"pwd\" id=\"pwd\">");
  htmlPage.concat("<input type=\"submit\" value=\"Connect\"></input></form>");
  htmlPage.concat("</body></html>");
  server.send(200, "text/html",htmlPage);
}
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  //server.send(200, "text/html", "<h1>You are connected</h1>");
  server.send(200, "text/html",configPage);
}

void loadConfig(){
  //read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");
  hubConfig.mode = (uint8_t)(EEPROM.read(0));
    Serial.print("Mode: ");
  Serial.println(hubConfig.mode);
  int usedByteCount = sizeof(hubConfig.mode);

  String esid="";
  for (int i = usedByteCount; i < usedByteCount + sizeof(hubConfig.ssid); ++i)
    {
      if(EEPROM.read(i) != 0){
        esid += char(EEPROM.read(i));
      }
    }
  
  Serial.print("SSID: ");
  Serial.print(esid);Serial.println("\\END");  
   Serial.print("esid length: ");
  Serial.print(esid.length());
  Serial.println("Reading EEPROM pass");
  String epass = "";
  usedByteCount += sizeof(hubConfig.ssid);
  for (int i = usedByteCount; i < sizeof(hubConfig); ++i)
    {
      epass += char(EEPROM.read(i));
    }
  Serial.print("PASS: ");
  Serial.println(epass);  
  Serial.println("\\END");  
  //if ( esid.length() > 1 ) {
  esid.trim();
 
  strcpy(hubConfig.ssid,esid.c_str());
  strcpy(hubConfig.pwd,epass.c_str());

}
void storeConfig(){
  Serial.println("Storing configuration");
  Serial.print("Mode: ");
  Serial.print(hubConfig.mode);
  Serial.print(" SSID: ");
  Serial.print(hubConfig.ssid);
  Serial.print(" PWD: ");
  Serial.println(hubConfig.pwd);
  Serial.println("clearing eeprom");
  
  
  for (int i = 0; i < sizeof(hubConfig); ++i) { EEPROM.write(i, 0); }
  
  EEPROM.write(0, hubConfig.mode);
  int usedByteCount = sizeof(hubConfig.mode);
  String qsid = ""; 
  qsid.concat(hubConfig.ssid);
  Serial.println(qsid);
  Serial.println("");
  String qpass="";
  qpass.concat(hubConfig.pwd);
  Serial.println(qpass);
  Serial.println("");
  
  Serial.println("writing eeprom ssid:");
  for (int i = 0; i < qsid.length(); ++i)
    {
      EEPROM.write(usedByteCount+i, qsid[i]);
      Serial.print("Wrote: ");
      Serial.println(qsid[i]); 
    }

   usedByteCount += sizeof(hubConfig.ssid);
  Serial.println("writing eeprom pass:"); 
  for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(usedByteCount+i, qpass[i]);
      Serial.print("Wrote: ");
      Serial.println(qpass[i]); 
    }    
  EEPROM.commit();
}
void clearStoredConfig(){
  Serial.println("\nclearing eeprom");
        for (int i = 0; i < sizeof(hubConfig); ++i) { EEPROM.write(i, 0); }

  EEPROM.commit();
}

