// Reset.h

#ifndef _RESET_h
#define _RESET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "IState.h"

class Reset : public IState
{
public:
	// Inherited via IState
	virtual void Begin() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void UpButtonPress() override;
	virtual void DownButtonPress() override;

private:
	bool reset = false;
	String getAnswer();
	void doPress();
};

#endif

