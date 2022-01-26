// ShtSensor.h

#ifndef _SHTSENSOR_h
#define _SHTSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"
#include "ISensor.h"
#include <Adafruit_SHT31.h>
#include "HeaterObserver.h"

// MODIFIED SOURCE:
// delay(500) in source is much too long
// Adafruit_SHT31.cpp:
//	comment line 74:
//		delay(500);
//	add:
//		vTaskDelay(15 / portTICK_PERIOD_MS);


class ShtSensor : public ISensor, public HeaterObserver
{
public:
	// Inherited via ISensor
	virtual void Begin(int number, uint8_t address) override;
	virtual void Begin(int number, int pin) override;
	virtual int Number() override;
	virtual float Humidity() override;
	virtual float Temperature() override;
	virtual float Pressure() override;
	virtual float Altitude() override;

	// Inherited via HeaterObserver
	virtual void HeaterState(int h) override;

private:
	Adafruit_SHT31 sht;
};

#endif

