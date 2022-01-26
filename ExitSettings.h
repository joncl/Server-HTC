// ExitSettings.h

#ifndef _EXITSETTINGS_h
#define _EXITSETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "IState.h"
#include "ExitStateObserver.h"

class ExitSettings : public IState
{
public:
	// Inherited via IState
	virtual void Begin() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void UpButtonPress() override;
	virtual void DownButtonPress() override;

private:
	bool input = false;
	String getAnswer();
	void doPress();
};

#endif

