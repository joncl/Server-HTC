// 
// 
// 

#include "Relay.h"

void Relay::Begin()
{
	pinMode(RELAY_LED_PIN, OUTPUT);
	pinMode(RELAY_LED_LOCAL_PIN, OUTPUT);
	ObserverManager::subscribe<SensorDataObserver>(this);
}

void Relay::SensorData(float h, float t)
{
	if (Settings::GetRelayMode() == 0 && h < Settings::GetThresholds()[state]
		||
		Settings::GetRelayMode() == 1 && h > Settings::GetThresholds()[state])
	{
		digitalWrite(RELAY_LED_PIN, HIGH);
		digitalWrite(RELAY_LED_LOCAL_PIN, HIGH);

		state = 1;
	}
	else
	{
		digitalWrite(RELAY_LED_PIN, LOW);
		digitalWrite(RELAY_LED_LOCAL_PIN, LOW);

		state = 0;
	}

	int i;
	if (xQueueReceive(RequestRelayLedQueue, &i, 0))
	{
		IndexValue iv = { (int)Led::Relay, state };
		xQueueSend(SendLedQueue, (void*)&iv, 0);
	}
}

