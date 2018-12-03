#include "Sniffer_Dust_Sensor.h"
#include <SoftwareSerial.h>
#define NOVA_RX 8
#define NOVA_TX 7

//#include <SPI.h>
SnifferDustSensor dustSensor;
void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  // put your setup code here, to run once:
  //initNovaSenor(&mySerial);
  dustSensor.begin(NOVA_RX,NOVA_TX);
  Serial.println("Ending");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Reading Data");
  dustSensor.readDustData();
  Serial.println("Ending read Data");
  
  Serial.print ("PM10:");
  Serial.println(dustSensor.getPM10());
  Serial.print ("PM2.5:");
  Serial.println(dustSensor.getPM25());
  Serial.println("************************");

delay(2000);
}