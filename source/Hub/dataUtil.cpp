#include "dataUtil.h"
#include "properties.h"
#include "constants.h"
#include <ArduinoJson.h>

void convertAirData(String dataStr){
  int index = 0, nextIndex;
  String subStr;

  nextIndex = dataStr.indexOf(',',index);
  if(nextIndex > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.novaPm25 = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.novaPm10 = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.temperature = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    envData.humidity = subStr.toFloat();
  }
}
void formatAAVNData(char * dataStr){
  StaticJsonBuffer<500> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  JsonObject& source = root.createNestedObject(SOURCE_KEY);
  source[SOURCE_ID_KEY] = sender;
  //String macStr = WiFi.macAddress();
  //macStr.replace(":","-");
  source[MAC_KEY] = macStr;
  //source[GPS_KEY] = gps;
  JsonObject& gps = source.createNestedObject(GPS_KEY);
  gps[LAT_KEY] = latitude;
  gps[LONG_KEY]= longitude;


  JsonArray& data = root.createNestedArray(VALS_KEY);
  
  StaticJsonBuffer<150> buffer1;
  JsonObject& obj1 = buffer1.createObject();
  if(envData.humidity >= HUM_MIN && envData.humidity <= HUM_MAX){
    obj1[CODE_KEY] = HUM_KEY;
    obj1[SENSOR_KEY] = TEMP_SENSOR;
    JsonObject& val1 = obj1.createNestedObject(VAL_KEY);
    val1[VAL_KEY] = envData.humidity;
    data.add(obj1);
  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading HUM: ");
    Serial.println(envData.humidity);
    Serial.println("/*******************************/");
  }
  
  StaticJsonBuffer<150> buffer2;
  JsonObject& obj2 = buffer2.createObject();

  if(envData.temperature > TEMP_MIN && envData.temperature <= TEMP_MAX){
    obj2[CODE_KEY] = TEMP_KEY;
    obj2[SENSOR_KEY] = TEMP_SENSOR;
    JsonObject& val2 = obj2.createNestedObject(VAL_KEY);
    val2[VAL_KEY] = envData.temperature;
    data.add(obj2);

  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading TEMP: ");
    Serial.println(envData.temperature);
    Serial.println("/*******************************/");
  }
  
  StaticJsonBuffer<150> buffer3;
  JsonObject& obj3 = buffer3.createObject();

  if(envData.novaPm25 >= PM_MIN && envData.novaPm25 <= PM_MAX){
      obj3[CODE_KEY] = PM25_KEY;
      obj3[SENSOR_KEY] = PM_SENSOR;
      JsonObject& val3 = obj3.createNestedObject(VAL_KEY);
      val3[VAL_KEY] = envData.novaPm25;
      
  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading PM2.5: ");
    Serial.println(envData.novaPm25);
    Serial.println("/*******************************/");
  }

  StaticJsonBuffer<150> buffer4;
  JsonObject& obj4 = buffer4.createObject();

  if(envData.novaPm10 >= PM_MIN && envData.novaPm10 <= PM_MAX){
    obj4[CODE_KEY] = PM10_KEY;
    obj4[SENSOR_KEY] = PM_SENSOR;
    JsonObject& val4 = obj4.createNestedObject(VAL_KEY);
    val4[VAL_KEY] = envData.novaPm10;
    
  }else{
    Serial.println("/*******************************/");
    Serial.print("ERROR reading PM10: ");
    Serial.println(envData.novaPm10);
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
  String returnStr = "";
  root.prettyPrintTo(Serial);
  root.printTo(dataStr,500);
  //return returnStr;
}


 //check valid data 
 bool isValidAirData(){
   return (envData.novaPm25>0)&& (envData.novaPm10>0) && (envData.temperature>0) && (envData.humidity>0);
 }



