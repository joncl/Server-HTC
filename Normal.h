// Normal.h

#ifndef _NORMAL_h
#define _NORMAL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "IState.h"
#include "SensorDataObserver.h"

class Normal : public IState
{
public:
	
	// Inherited via IState
	virtual void Begin() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void UpButtonPress() override;
	virtual void DownButtonPress() override;

	void Loop();
};

#endif

