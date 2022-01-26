// Heater.h

#ifndef _HEATER_h
#define _HEATER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "IState.h"
#include "HeaterObserver.h"
#include "HeaterSettingObserver.h"

class Heater : public IState, public HeaterSettingObserver
{
public:
	// Inherited via IState
	virtual void Begin() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void UpButtonPress() override;
	virtual void DownButtonPress() override;

	void Loop();

	// Inherited via HeatSettingObserver
	virtual void HeaterState(int h) override;

private:
	bool active = false;
	int heaterState = 0;
	unsigned long counter;
	String getAnswer();
	void doPress();
	void stateChanged();
};

#endif

