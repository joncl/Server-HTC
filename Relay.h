// Relay.h

#ifndef _RELAY_h
#define _RELAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"
#include "SensorDataObserver.h"
#include "Settings.h"

class Relay : public SensorDataObserver
{
public:
	void Begin();

	// Inherited via SensorObserver
	virtual void SensorData(float h, float t) override;

private:
	int state = 0;
};

#endif

