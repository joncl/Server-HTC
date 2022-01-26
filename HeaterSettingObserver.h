// HeaterSettingObserver.h

#ifndef _HEATERSETTINGOBSERVER_h
#define _HEATERSETTINGOBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#endif

#include "Globals.h"

class HeaterSettingObserver : public ObserverBase
{
public:
	virtual void HeaterState(int h) = 0;
};

#endif