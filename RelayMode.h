// RelayMode.h

#ifndef _RELAYMODE_h
#define _RELAYMODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "IState.h"

class RelayMode : public IState
{
public:
	// Inherited via IState
	virtual void Begin() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void UpButtonPress() override;
	virtual void DownButtonPress() override;

private:
	int relayMode;
	String getOutput();
};

#endif

