// 
// 
// 

#include "StateManager.h"


void StateManager::Begin()
{
	//Serial.println("stateManager begin");

	// add states to map
	stateMap[State::Normal] = &normal;
	stateMap[State::Set] = &set;
	stateMap[State::RelayMode] = &relayMode;
	stateMap[State::Hysteresis] = &hysteresis;
	stateMap[State::Calibrate] = &calibrate;
	stateMap[State::TempUnit] = &tempUnit;
	stateMap[State::Heater] = &heater;
	stateMap[State::Sensors] = &sensors;
	stateMap[State::Exit] = &exit;
	stateMap[State::Reset] = &reset;

	Settings::Begin();

	// initialize states
	normal.Begin();
	set.Begin();
	relayMode.Begin();
	hysteresis.Begin();
	calibrate.Begin();
	tempUnit.Begin();
	heater.Begin();
	sensors.Begin();
	exit.Begin();
	reset.Begin();

	// initialize display
	display.Begin();

	// enter first state Normal
	enterState(State::Normal);
}

void StateManager::Loop()
{
	if (currentState == State::Normal || currentState == State::Sensors)
	{
		sensors.Loop();
	}
	Settings::Loop();
	heater.Loop();
}

void StateManager::ResetLongPress()
{
	//Serial.println("rcv long push stmgr");
	enterState(State::Reset);
}

void StateManager::SetLongPress()
{
	//Serial.println("settings push");
	stateMap[currentState]->Exit();
	enterState(State::RelayMode);
}

void StateManager::SettingsExit(bool doExit)
{
	exitSettings = doExit;
}

void StateManager::enterState(State state)
{
	stateMap[state]->Enter();
	currentState = state;
}

void StateManager::exitState(State state)
{
	stateMap[state]->Exit();
	if (state == State::Reset || state == State::Exit)
		ObserverManager::notify(&StateManagerObserver::ExitLongPressState);
}

bool StateManager::changeState(ButtonName name)
{
	if (currentState == State::Exit && name == ButtonName::Set)
	{
		newState = exitSettings ? State::Normal : State::RelayMode;
	}
	else
	{
		newState = static_cast<State>(stateChangeMap[(int)currentState][(int)name]);
	}
	if (currentState == newState) return false;
	exitState(currentState);
	enterState(newState);
	return true;
}

void StateManager::ButtonPress(ButtonName name)
{
	byte permissionTest = buttonBits[(int)name] & buttonPermissions[(int)currentState];
	if (permissionTest > 0)
	{
		stateChanged = changeState(name);
		if (!stateChanged)
		{
			switch (name)
			{
			case ButtonName::Up:
				stateMap[currentState]->UpButtonPress();
				break;
			case ButtonName::Down:
				stateMap[currentState]->DownButtonPress();
				break;
			}
		}
	}
}

const char StateManager::buttonBits[4] =
{
	0b00001000,			// Reset button
	0b00000100,			// Set button
	0b00000010,			// Up button
	0b00000001			// Down button
};

const char StateManager::buttonPermissions[10] =
{
	0b00000011,			// 0 - Normal
	0b00000111,			// 1 - Set
	0b00001111,			// 2 - RelayMode
	0b00001111,			// 3 - Hysteresis
	0b00001111,			// 4 - Calibrate
	0b00001111,			// 5 - TempUnit
	0b00001111,			// 6 - Heater
	0b00001111,			// 7 - Sensors
	0b00001111,			// 8 - Exit
	0b00000111			// 9 - Reset
};

//const int StateManager::stateChangeMap[9][4] =
//{
//	// for single button presses
//	// index is state
//	// [0] is Relay button
//	// [1] is Set button
//	// [2] is Up button
//	// [3] is Down button
//	// Up and Down buttons do not change state
//	 0, 0, 1, 1 },		// 0 - Normal
//	 1, 0, 1, 1 },		// 1 - Set
//	 7, 3, 2, 2 },		// 2 - RelayMode
//	 2, 4, 3, 3 },		// 3 - Hysteresis
//	 3, 5, 4, 4 },		// 4 - Calibrate
//	 4, 6, 5, 5 },		// 5 - TempUnit
//	 5, 7, 6, 6 },		// 6 - Sensors
//	 6, 7, 7, 7 },		// 7 - Exit
//	 8, 0, 8, 8 }		// 8 - Reset
//};

const Matrix<int, 10, 4> StateManager::stateChangeMap =
{ {
		// for single button presses
		// index is state
		// [0] is Relay button
		// [1] is Set button
		// [2] is Up button
		// [3] is Down button
		// Up and Down buttons do not change state
		{ 0, 0, 1, 1 },		// 0 - Normal
		{ 1, 0, 1, 1 },		// 1 - Set
		{ 8, 3, 2, 2 },		// 2 - RelayMode
		{ 2, 4, 3, 3 },		// 3 - Hysteresis
		{ 3, 5, 4, 4 },		// 4 - Calibrate
		{ 4, 6, 5, 5 },		// 5 - TempUnit
		{ 5, 7, 6, 6 },		// 6 - Heater
		{ 6, 8, 7, 7 },		// 7 - Sensors
		{ 7, 8, 8, 8 },		// 8 - Exit
		{ 9, 0, 9, 9 }		// 9 - Reset
} };

