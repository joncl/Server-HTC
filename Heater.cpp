// 
// 
// 

#include "Heater.h"

void Heater::Begin()
{
	pinMode(HEATER_LED_PIN, OUTPUT);
	ObserverManager::subscribe<HeaterSettingObserver>(this);
}

void Heater::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "HEAT?", FontType::ExtraCondensed);
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getAnswer(), FontType::Condensed);
	active = true;
}

void Heater::Exit()
{
	stateChanged();
	active = false;
}

void Heater::UpButtonPress()
{
	doPress();
}

void Heater::DownButtonPress()
{
	doPress();
}

void Heater::Loop()
{
	if (heaterState && !active)
	{
		unsigned long newMillis = millis();
		if ((newMillis - counter) >= HEAT_TIME_ON * 1000)
		{
			heaterState = false;
			stateChanged();
		}
	}

	int i;
	if (xQueueReceive(RequestHeaterLedQueue, &i, 0))
	{
		IndexValue iv = { (int)Led::Heater, heaterState };
		xQueueSend(SendLedQueue, (void*)&iv, 0);
	}
}

void Heater::doPress()
{
	heaterState = !heaterState;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getAnswer(), FontType::Normal);
}

void Heater::stateChanged()
{
	if (heaterState)
	{
		digitalWrite(HEATER_LED_PIN, HIGH);

		// reset the counter if heat is on
		counter = millis();
	}
	else
	{
		digitalWrite(HEATER_LED_PIN, LOW);
	}

	ObserverManager::notify(&HeaterObserver::HeaterState, heaterState);

	IndexValue lv = { (int)Led::Heater, heaterState };
	xQueueSend(SendLedQueue, (void*)&lv, 0);
}

// Inherited via HeatSettingObserver
void Heater::HeaterState(int h)
{
	heaterState = h;
	stateChanged();
}

String Heater::getAnswer()
{
	if (!heaterState) return "OFF";
	if (heaterState) return "ON";
}



