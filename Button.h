// Button.h

#ifndef _BUTTON_h
#define _BUTTON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "Globals.h"
#include "ButtonObserver.h"

class Button
{
public:
	void Begin(ButtonName n, int p);
	void Loop();
	void SetInterrupted();

private:
	ButtonName name;
	int pin;

	// constants
	static const int debounceInterval = 20;
	static const int longPressMillisMax = 1000;	// seconds

	// vars for debounce
	volatile int interrupted = 0;
	volatile unsigned long previousMillis = 0;
	volatile int current = 0;
	volatile int previous = 0;

	// flags
	volatile int shortPress = 0;
	volatile int shortPressNotified = 0;
	volatile int longPress = 0;
	volatile unsigned long longPressMillis = 0;
	volatile int longPressInProgess = 0;
	volatile int longPressNotified = 0;
	volatile int longPressReleasedNotified = 1;
};

#endif

