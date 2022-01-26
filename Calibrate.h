// Calibrate.h

#ifndef _CALIBRATE_h
#define _CALIBRATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "IState.h"

class Calibrate : public IState
{
public:
	// Inherited via IState
	virtual void Begin() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void UpButtonPress() override;
	virtual void DownButtonPress() override;

private:
	int calibration;
};

#endif
