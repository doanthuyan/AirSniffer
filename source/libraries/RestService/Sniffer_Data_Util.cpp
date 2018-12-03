#include "Sniffer_Data_Util.h"
#include "Sniffer_Rest_Constant.h"
#include <ArduinoJson.h>
#include <Sniffer_Rest_Property.h>

void formatAAVNData(char * dataStr, Environment envirData,RestProperty restProperty){
	
  StaticJsonBuffer<700> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  JsonObject& source = root.createNestedObject(SOURCE_KEY);
  source[SOURCE_ID_KEY] = restProperty.sender_pro;
  //String macStr = WiFi.macAddress();
  //macStr.replace(":","-");
  source[MAC_KEY] = restProperty.mac_str_pro;
  //source[MAC_KEY] = "FF-A2-DA-00-3F-00";
  //source[GPS_KEY] = gps;
  JsonObject& gps = source.createNestedObject(GPS_KEY);
  gps[LAT_KEY] = restProperty.latitude_pro;
  gps[LONG_KEY]= restProperty.longitude_pro;


  JsonArray& data = root.createNestedArray(VALS_KEY);
  
  StaticJsonBuffer<200> buffer1;
  JsonObject& obj1 = buffer1.createObject();
  if(envirData.humidity >= HUM_MIN && envirData.humidity <= HUM_MAX){
    obj1[CODE_KEY] = HUM_KEY;
    obj1[SENSOR_KEY] = restProperty.TEMP_SENSOR_pro;
    JsonObject& val1 = obj1.createNestedObject(VAL_KEY);
    val1[VAL_KEY] = envirData.humidity;
    data.add(obj1);
  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading HUM: ");
    Serial.println(envirData.humidity);
    Serial.println("/*******************************/");
  }
  

  
  StaticJsonBuffer<200> buffer2;
  JsonObject& obj2 = buffer2.createObject();

  if(envirData.temperature >= TEMP_MIN && envirData.temperature <= TEMP_MAX){
    obj2[CODE_KEY] = TEMP_KEY;
    obj2[SENSOR_KEY] = restProperty.TEMP_SENSOR_pro;
    JsonObject& val2 = obj2.createNestedObject(VAL_KEY);
    val2[VAL_KEY] = envirData.temperature;
    data.add(obj2);

  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading TEMP: ");
    Serial.println(envirData.temperature);
    Serial.println("/*******************************/");
  }
  
  StaticJsonBuffer<200> buffer3;
  JsonObject& obj3 = buffer3.createObject();

  if(envirData.novaPm25 >= PM_MIN && envirData.novaPm25 <= PM_MAX){
      obj3[CODE_KEY] = PM25_KEY;
      obj3[SENSOR_KEY] = restProperty.PM_SENSOR_pro;
      JsonObject& val3 = obj3.createNestedObject(VAL_KEY);
      val3[VAL_KEY] = envirData.novaPm25;
      
  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading PM2.5: ");
    Serial.println(envirData.novaPm25);
    Serial.println("/*******************************/");
  }

  StaticJsonBuffer<200> buffer4;
  JsonObject& obj4 = buffer4.createObject();

  if(envirData.novaPm10 >= PM_MIN && envirData.novaPm10 <= PM_MAX){
    obj4[CODE_KEY] = PM10_KEY;
    obj4[SENSOR_KEY] = restProperty.PM_SENSOR_pro;
    JsonObject& val4 = obj4.createNestedObject(VAL_KEY);
    val4[VAL_KEY] = envirData.novaPm10;
    
  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading PM10: ");
    Serial.println(envirData.novaPm10);
    Serial.println("/*******************************/");
  }

  /*Serial.println();
  obj1.prettyPrintTo(Serial);
  Serial.println();
  obj2.prettyPrintTo(Serial);
  */
  
  
  
  data.add(obj3);
  data.add(obj4);

  Serial.println();
  root.prettyPrintTo(Serial);
  root.printTo(dataStr,600);

}


 //check valid data 
 bool isValidAirData(Environment envirData){
   return (envirData.novaPm25>0)&& (envirData.novaPm10>0) && (envirData.temperature>0) && (envirData.humidity>0);
 }

void printData(Environment envirData){
  int index=0;
  index=index+1;
  Serial.print(index );
  Serial.print(" Nova_PM2.5: ");
  Serial.print(envirData.novaPm25);
  Serial.print(" Nova_PM10: ");
  Serial.print(envirData.novaPm10);
  Serial.print(" Temperature: ");
  Serial.print(envirData.temperature);
  Serial.print(" Humidity: ");
  Serial.print(envirData.humidity);
  Serial.println();
}
