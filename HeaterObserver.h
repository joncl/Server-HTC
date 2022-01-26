// HeaterObserver.h

#ifndef _HEATEROBSERVER_h
#define _HEATEROBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"

class HeaterObserver : public ObserverBase
{
public:
	virtual void HeaterState(int h) = 0;
};

#endif
