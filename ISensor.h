// ISensor.h

#ifndef _ISENSOR_h
#define _ISENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#endif

#include "Globals.h"

class ISensor
{
public:
	virtual void Begin(int number, uint8_t address) = 0;
	virtual void Begin(int number, int pin) = 0;
	virtual int Number() = 0;
	virtual float Humidity() = 0;
	virtual float Temperature() = 0;;
	virtual float Pressure() = 0;
	virtual float Altitude() = 0;

protected:
	int _number;
};

#endif