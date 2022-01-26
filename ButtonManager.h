// ButtonManager.h

#ifndef _BUTTON_MANAGER_h
#define _BUTTON_MANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "ButtonObserver.h"
#include "Button.h"
#include "ButtonManagerObserver.h"
#include "StateManagerObserver.h"

enum class InputState : int { Normal, LongPress };

class ButtonManager : public ButtonObserver, public StateManagerObserver
{
public:
	void Begin();
	void Loop();

	// Inherited via ButtonObserver
	void ShortPress(ButtonName label) override;
	void LongPress(ButtonName label) override;
	void LongPressReleased() override;

	// Inherited via StateManagerObserver
	void ExitLongPressState() override;

private:
	static Button button1;
	static Button button2;
	static Button button3;
	static Button button4;

	InputState inputState;

	static void button1Interrupt();
	static void button2Interrupt();
	static void button3Interrupt();
	static void button4Interrupt();

	bool longPressInProgress = false;
	bool longPressInPregressDisplayUpdated = false;

};


#endif

