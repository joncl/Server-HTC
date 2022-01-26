// IState.h

#ifndef _ISTATE_h
#define _ISTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <iostream>
#include <functional>
#include <map>
#include <Preferences.h>
#include "Globals.h"
#include "DisplayContentObserver.h"

template <class T, size_t ROW, size_t COL>
using Matrix = std::array<std::array<T, COL>, ROW>;

class IState
{
public:
	virtual void Begin() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void UpButtonPress() = 0;
	virtual void DownButtonPress() = 0;
};

#endif

