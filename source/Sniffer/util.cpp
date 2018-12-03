#include "util.h"
#include "properties.h"

void printData(){
  int index=0;
  index=index+1;
  Serial.print(index );
  Serial.print(" Nova_PM2.5: ");
  Serial.print(envData.novaPm25);
  Serial.print(" Nova_PM10: ");
  Serial.print(envData.novaPm10);
  Serial.print(" Temperature: ");
  Serial.print(envData.temperature);
  Serial.print(" Humidity: ");
  Serial.print(envData.humidity);
  Serial.println();
}
