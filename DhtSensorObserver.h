// DhtSensorObserver.h

#ifndef _DHTSENSOROBSERVER_h
#define _DHTSENSOROBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"

class DhtSensorObserver : public ObserverBase
{
public:
	virtual void ReceiveSensorData(float h, float t) = 0;
};


#endif
