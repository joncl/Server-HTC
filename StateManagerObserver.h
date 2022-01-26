// StateManagerObserver.h

#ifndef _STATEMANAGEROBSERVER_h
#define _STATEMANAGEROBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

class StateManagerObserver : public ObserverBase
{
public:
	virtual void ExitLongPressState() = 0;
};

#endif
