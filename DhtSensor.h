// DhtSensor.h

#ifndef _DHTSENSOR_h
#define _DHTSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ISensor.h"
#include <DHTesp.h>

class DhtSensor : public ISensor
{
public:
	// Inherited via ISensor
	void Begin(int number, uint8_t address) override;
	void Begin(int number, int pin) override;
	int Number() override;
	float Humidity() override;
	float Temperature() override;
	float Pressure() override;
	float Altitude() override;

private:
	DHTesp dht;

};
#endif

