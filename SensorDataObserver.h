// SensorDataObserver.h

#ifndef _SENSORDATAOBSERVER_h
#define _SENSORDATAOBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#endif

#include "Globals.h"

class SensorDataObserver : public ObserverBase
{
public:
	virtual void SensorData(float h, float t) = 0;
};

#endif