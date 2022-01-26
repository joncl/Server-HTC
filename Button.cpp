// 
// 
// 

#include "Button.h"


void Button::Begin(ButtonName n, int p) {
	name = n;
	pin = p;
}

void IRAM_ATTR Button::SetInterrupted() {
	interrupted = 1;
}

void Button::Loop()
{
	if (shortPress == 1)
	{
		if (shortPressNotified == 0)
		{
			ObserverManager::notify(&ButtonObserver::ShortPress, name);
			shortPressNotified == 1;
			shortPress = 0;
		}
	}
	if (longPress == 1 && longPressInProgess == 1)
	{
		if (longPressNotified == 0)
		{
			ObserverManager::notify(&ButtonObserver::LongPress, name);
			longPressNotified = 1;
		}
	}
	if (longPress == 0 && longPressInProgess == 0)
	{
		if (longPressReleasedNotified == 0)
		{
			ObserverManager::notify(&ButtonObserver::LongPressReleased);
			longPressReleasedNotified = 1;
			longPress = 0;
		}
	}

	if (interrupted == 1)
	{
		if (millis() - previousMillis > Button::debounceInterval)
		{
			previousMillis = millis();
			current = digitalRead(pin);

			// CASE: button press (low to high)
			if (current == HIGH && previous == LOW)
			{
				longPress = longPressMillis = 0;
				shortPress = 1;
			}
			// CASE: button release (high to low)
			else if (current == LOW && previous == HIGH)
			{
				shortPress = shortPressNotified = 0;
				longPress = longPressNotified = longPressInProgess = interrupted = 0;
			}
			// CASE: long pressed still held
			else if (current == HIGH && longPressInProgess == 1)
			{
				return;		// do nothing
			}
			// CASE: long press
			else if (current == HIGH && longPress == 0)
			{
				if (longPressMillis == 0) longPressMillis = millis();
				//Serial.println("lpc:" + String(millis() - longPressMillis));
				if (millis() - longPressMillis >= Button::longPressMillisMax)
				{
					longPress = 1;
					longPressInProgess = 1;
					longPressReleasedNotified = 0;
					longPressMillis = 0;
					//Serial.println("LONG PRESS " + String((int)name));
				}
			}
			// remember button state for next loop
			previous = current;
		}
	}
}