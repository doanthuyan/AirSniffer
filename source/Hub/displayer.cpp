#include "displayer.h"
#include "properties.h"

#include "Sniffer_Smart_Config.h"

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif
const char degree = char (223);
const char micro = char (241);
#define CUBIC_INDEX 0

byte cubic[8]={
B00111,
B00001,
B00011,
B00001,
B00111,
B00000,
B00000,
B00000
};
void initLcd(){
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("SNIFFER!");
  lcd.setCursor(0,1);
  lcd.print("Starting ...");
  lcd.createChar(CUBIC_INDEX,cubic);

  
}
void showAirData(Environment *envData){

  char humStr[8];
  char tempStr[8];
  
  dtostrf(envData->humidity, 3, 0, humStr);
  dtostrf(envData->temperature, 2, 0, tempStr);

  char line1[16];
  sprintf(line1,"Temperature:%s%cC",tempStr,degree);
  char line12[16];
  sprintf(line12,"Humidity:%s%%",humStr);


  // Show temperature info
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line12);
  
  Serial.println(line1);
  Serial.println(line12);
  delay(5000); // delay 5s to show more information with 2-lines LCD
  //Then show PM data
  
  
  char pm25Str[8];
  char pm10Str[8];
  char line2[16];
  char line22[16];
  dtostrf(envData->novaPm25, 3, 1, pm25Str);
  dtostrf(envData->novaPm10, 3, 1, pm10Str);

  sprintf(line2, "PM2.5:%s ug/m",pm25Str);
  sprintf(line22,"PM10:%s ug/m",pm10Str);
  
  lcd.clear(); //For LCD 2 lines  
  lcd.setCursor(0,0);   // For LCD 2 lines  
  //lcd.setCursor(0,2); // For LCD 4 Lines
  lcd.print(line2);
  lcd.printByte(CUBIC_INDEX);
  
  lcd.setCursor(0,1); // For LCD 2 lines  
  //lcd.setCursor(0,3); // For LCD 4 Lines
  lcd.print(line22);
  lcd.printByte(CUBIC_INDEX);
  
  Serial.println(line2);
  Serial.println(line22);
}
void showSmartConfig(){
  
  lcd.clear();
    
  lcd.setCursor(0,0);
  lcd.print("Config Steps");
  
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("1.Connect Wifi:");
  lcd.setCursor(0,1);
  lcd.print(smartConfigSSID);

  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("2.Open browser:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.softAPIP());
  delay(2000);

}
void convertAirData(String dataStr,Environment * data){
  int index = 0, nextIndex;
  String subStr;

  nextIndex = dataStr.indexOf(',',index);
  if(nextIndex > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    data->novaPm25 = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    data->novaPm10 = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    data->temperature = subStr.toFloat();
  }
  index = nextIndex + 1;
  nextIndex = dataStr.indexOf(',',index);
   if(index > 0){
    subStr = dataStr.substring(index,nextIndex);
    //Serial.print("Next index: ") ; Serial.println(nextIndex);
    //Serial.println(subStr);
    data->humidity = subStr.toFloat();
  }
}

void askRestart(){
  lcd.clear();
    
  lcd.setCursor(0,0);
  lcd.print("Config Done!");
  delay (1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please restart");
  lcd.setCursor(0,1);
  lcd.print("  Sniffer!");
}

