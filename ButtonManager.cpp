// 
// 
// 

#include "ButtonManager.h"


void ButtonManager::ShortPress(ButtonName name) // from Observer
{
	ObserverManager::notify(&ButtonManagerObserver::ButtonPress, name);
}

void ButtonManager::LongPress(ButtonName name) // from Observer
{
	if (name == ButtonName::Reset || name == ButtonName::Set)
	{
		longPressInProgress = true;
		if (inputState == InputState::Normal)
		{
			inputState = InputState::LongPress;
			switch(name)
			{
				case ButtonName::Reset:
					ObserverManager::notify(&ButtonManagerObserver::ResetLongPress);
					break;
				case ButtonName::Set:
					ObserverManager::notify(&ButtonManagerObserver::SetLongPress);
					break;
			}
		}
	}
}

void ButtonManager::LongPressReleased()
{
	longPressInProgress = false;
}

void ButtonManager::ExitLongPressState()
{
	inputState = InputState::Normal;
}

void ButtonManager::Begin()
{
	//Serial.println("buttonManager begin");
	inputState = InputState::Normal;

	ButtonManager::button1.Begin(ButtonName::Reset, BUTTON_RESET_PIN);
	pinMode(BUTTON_RESET_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(BUTTON_RESET_PIN), ButtonManager::button1Interrupt, RISING);

	ButtonManager::button2.Begin(ButtonName::Set, BUTTON_SET_PIN);
	pinMode(BUTTON_SET_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(BUTTON_SET_PIN), ButtonManager::button2Interrupt, RISING);

	ButtonManager::button3.Begin(ButtonName::Up, BUTTON_UP_PIN);
	pinMode(BUTTON_UP_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(BUTTON_UP_PIN), ButtonManager::button3Interrupt, RISING);

	ButtonManager::button4.Begin(ButtonName::Down, BUTTON_DOWN_PIN);
	pinMode(BUTTON_DOWN_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN_PIN), ButtonManager::button4Interrupt, RISING);

	ObserverManager::subscribe<ButtonObserver>(this);
}

void IRAM_ATTR ButtonManager::button1Interrupt() { ButtonManager::button1.SetInterrupted(); }
void IRAM_ATTR ButtonManager::button2Interrupt() { ButtonManager::button2.SetInterrupted(); }
void IRAM_ATTR ButtonManager::button3Interrupt() { ButtonManager::button3.SetInterrupted(); }
void IRAM_ATTR ButtonManager::button4Interrupt() { ButtonManager::button4.SetInterrupted(); }

void ButtonManager::Loop()
{
	ButtonManager::button1.Loop();
	ButtonManager::button2.Loop();
	ButtonManager::button3.Loop();
	ButtonManager::button4.Loop();
}

Button ButtonManager::button1;
Button ButtonManager::button2;
Button ButtonManager::button3;
Button ButtonManager::button4;

