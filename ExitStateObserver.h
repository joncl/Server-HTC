// ExitStateObserver.h

#ifndef _EXITSTATEOBSERVER_h
#define _EXITSTATEOBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"

class ExitStateObserver : public ObserverBase
{
public:
	virtual void SettingsExit(bool doExit) = 0;
};

#endif
