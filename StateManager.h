// StateManager.h

#ifndef _STATEMANAGER_h
#define _STATEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <iostream>
#include <map>
#include <functional>
#include <Preferences.h>
#include "Globals.h"
#include "ButtonManagerObserver.h"
#include "StateManagerObserver.h"
#include "ExitStateObserver.h"
#include "Display.h"
#include "Settings.h"
#include "Normal.h"
#include "Set.h"
#include "RelayMode.h"
#include "Hysteresis.h"
#include "Calibrate.h"
#include "TempUnit.h"
#include "Heater.h"
#include "Sensors.h"
#include "ExitSettings.h"
#include "Reset.h"

class StateManager :
	public ButtonManagerObserver,
	public ExitStateObserver
	
{
public:
	void Begin();
	void Loop();

	// Inherited via ButtonManagerObserver
	void ResetLongPress() override;
	void SetLongPress() override;
	void ButtonPress(ButtonName buttonName) override;

	// Inherited via SettingsExitReceiver
	virtual void SettingsExit(bool doExit) override;
	
private:
	State newState;
	bool stateChanged;
	std::map<State, IState*> stateMap;
	static const char buttonBits[4];
	static const char buttonPermissions[10];
	//static const int stateChangeMap[9][4];
	static const Matrix<int, 10, 4> stateChangeMap;
	
	void enterState(State state);
	void exitState(State state);
	bool changeState(ButtonName name);

	State currentState = State::Normal;
	bool exitSettings = false;

	Display display;

	// states
	Normal normal;
	Set set;
	RelayMode relayMode;
	Hysteresis hysteresis;
	Calibrate calibrate;
	TempUnit tempUnit;
	Heater heater;
	Sensors sensors;
	ExitSettings exit;
	Reset reset;
};

#endif

