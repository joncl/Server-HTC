// ButtonManagerObserver.h

#ifndef _BUTTONMANAGEROBSERVER_h
#define _BUTTONMANAGEROBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"

class ButtonManagerObserver : public ObserverBase
{
public:
	virtual void ResetLongPress() = 0;
	virtual void SetLongPress() = 0;
	virtual void ButtonPress(ButtonName name) = 0;
};

#endif

