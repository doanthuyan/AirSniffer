#ifndef _SNIFFER_DUST_H
#define _SNIFFER_DUST_H

#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "Sds011.h"

#define SAMPLES 10
struct Dust{
  float novaPm25;
  float novaPm10;
};
using namespace sds011;
class SnifferDustSensor
{
	public:
		SnifferDustSensor(void);
		void begin(uint8_t pin_rx, uint8_t pin_tx);
		void begin(HardwareSerial *hardSerial);
		
		bool readDustData();
		
		
		float getPM10();
		float getPM25();

	
	private:
		bool calcNovaPM();
	
		uint8_t _pin_rx, _pin_tx;
		Sds011 *sensor;	
		HardwareSerial *novaSerial;
		SoftwareSerial *softSerial;
		Dust envData;		

};

#endif