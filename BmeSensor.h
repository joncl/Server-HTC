// BmeSensor.h

#ifndef _BMESENSOR_h
#define _BMESENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ISensor.h"
#include "Adafruit_BME280.h"

#define SEALEVELPRESSURE_HPA	(1013.25)

class BmeSensor : public ISensor
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
	Adafruit_BME280 bme;

};

#endif

