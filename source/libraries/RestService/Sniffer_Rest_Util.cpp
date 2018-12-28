#include "Sniffer_Rest_Util.h"
#include <SPI.h>
#include <WiFiClientSecure.h>
#include "Sniffer_Data_Util.h"
#include <Sniffer_Rest_Property.h>

#define STATUS_201 "HTTP/1.1 201 Created"

bool saveData(Environment envirData, RestProperty restProperty)
{
  WiFiClientSecure client;
  // Attempt to make a connection to the remote server
  Serial.println("\nAttempt to make a connection to the remote server");
  if ( !client.connect(serverAddress, httpsPort) ) {
    Serial.println("connection failed");
	return false;
  }
  Serial.println("\n\n---------------------------------------------------------------------\n");
  Serial.println("REQUEST: \n" );
  // We now create a URI for the request
  //String postData = "{\"source\": {\"senderCode\": \"143253\",\"netAddress\": \"192.168.1.1\"},\"data\": [{\"symbolCode\": \"O3\",\"value\": 4.1}, {\"symbolCode\": \"PM2.5\",\"value\": 55}]}";
  char data[600];
  formatAAVNData(data, envirData,restProperty);
  Serial.print("Data: ");
  Serial.println(data);
  String len = String(strlen(data));
  String requestStr = String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + serverAddress + "\r\n" +
               "User-Agent: SnifferHub\r\n" +
               "Content-Length: " + len +"\r\n"+
               "Content-Type: application/json\r\n"+
               "Connection: close\r\n"+
               "\r\n"+
               data+
               "\r\n";
  Serial.println(requestStr);
  // This will send the request to the server
  client.print(requestStr);
  
  // If there are incoming bytes, print them
  Serial.println("\n\n---------------------------------------------------------------------\n");
  Serial.println("RESPONSE: \n" );
  // Read all the lines of the reply from server and print them to Serial
  int waitcount = 0;
  while (!client.available() && waitcount++ < 300) {
      Serial.println(".");
       delay(10);
  }
  String response = "";
  int count = 0;
  while ( client.available() ) {
    char c = client.read();
	response.concat(c);
    //Serial.print(c);
    count++;
  }
  Serial.println(response);
  client.stop();
  response.trim();
  if(response.startsWith(STATUS_201)){
	  return true;
  }else{
	  return false;
  }
}
