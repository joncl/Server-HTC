// ButtonObserver.h

#ifndef _BUTTONOBSERVER_h
#define _BUTTONOBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"
#include "ObserverManager.h"

class ButtonObserver : public ObserverBase
{
public:
	virtual void ShortPress(ButtonName name) = 0;
	virtual void LongPress(ButtonName name) = 0;
	virtual void LongPressReleased() = 0;
};

#endif

