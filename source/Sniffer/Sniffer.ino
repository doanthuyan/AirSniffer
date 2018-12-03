#include <SoftwareSerial.h>
#include "properties.h"
#include <SPI.h>
#include "util.h"
#include <dht.h>
#include <JeeLib.h> // Low power functions library
#include "Sniffer_Dust_Sensor.h"
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog

dht DHT;
#define DHT11_PIN 5


SoftwareSerial cc1101Serial(9, 10); // RX, TX
Environment envData;
SnifferDustSensor dustSensor;

void setup()
{
    bool clear = true;
    Serial.begin(9600 );

    Serial.println("Start Sniffer");
    
    cc1101Serial.begin(9600 );
    Serial.println("Starting");
    // put your setup code here, to run once:
    //initNovaSenor(&mySerial);
    dustSensor.begin(NOVA_RX,NOVA_TX);
    Serial.println("Ending");
    pinMode(SENSOR_ERR,OUTPUT);
    pinMode(LED_SIGNAL,OUTPUT);

   //setup Hudmidity
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    digitalWrite(LED_SIGNAL, HIGH);
    digitalWrite(SENSOR_ERR,LOW);
  
}

void loop()
{
    
    delay(5000);
    //read dust sensor value
    Serial.println("Reading Data");
    bool ok = dustSensor.readDustData();
    Serial.println("Ending read Data");
    //if(ok){
      envData.novaPm25 = dustSensor.getPM25();
      envData.novaPm10 = dustSensor.getPM10();
      
      Serial.print ("PM10:");
      Serial.println(dustSensor.getPM10());
      Serial.print ("PM2.5:");
      Serial.println(dustSensor.getPM25());
    
    //}
     
    //read 
    readTemperatureAndHumidity();
    while(envData.temperature <= 0 || envData.humidity <= 0){
      Serial.println("DHT11 sensor doesn't work properly");
      digitalWrite(SENSOR_ERR,HIGH);
      readTemperatureAndHumidity();  
      delay(5000);
    }
    printData();
    delay(1000);
    cc1101Serial.listen();

    
    if(isValidData()){
       digitalWrite(SENSOR_ERR,LOW);
       Serial.println("Send data to Hub");
       //blinking the led signal to send to hub
        for (byte i=0;i<10;++i){
          digitalWrite(LED_SIGNAL, LOW);
          delay (100);
          digitalWrite(LED_SIGNAL, HIGH);
          delay (100);
        }
        sendInfoToHub();
        //turn on led signal
        digitalWrite(LED_SIGNAL, HIGH);
      // delay(5000);
     //Sleepy::loseSomeTime(6000);
      
     
       sleepSomeTime();

    }
   
}


//read temperature and humidity from DHT11
void readTemperatureAndHumidity(){
  
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }


  envData.temperature = DHT.temperature;
  envData.humidity = DHT.humidity;
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  
  }

 //check valid data 
 bool isValidData(){
   return (envData.novaPm25>0)&& (envData.novaPm10>0) && (envData.temperature>0) && (envData.humidity>0);
 }


//send data for hub
void sendInfoToHub(){
  String theMessage = String(envData.novaPm25)+ "," + String(envData.novaPm10) +"," +String(envData.temperature)+ "," +  String(envData.humidity) ;
 const char* message = "hello";
  cc1101Serial.write(theMessage.c_str());
  delay(5000);
}


 //deep sleep
 void sleepSomeTime(){
    
  for (byte i = 0; i < SLEEP_PERIOD; ++i)
      Sleepy::loseSomeTime(60000); //maximum is 60 second for this method 
  
 }



