// 
// 
// 

#include "BlynkManager.h"

void BlynkManager::Begin()
{
	instance = this;

	// check Blynk connection every BLYNK_CONNECTION_CHECK_INTERVAL
	timer.setInterval(BLYNK_CONNECTION_CHECK_INTERVAL, BlynkManager::checkConnection);

	Blynk.config(auth);
	Blynk.disconnect();		// don't connect yet

	// do the first connect to Blynk in 2 seconds
	timer.setInterval(2000L, BlynkManager::checkConnection);

	//pinMode(LED_PIN, OUTPUT);
}

void BlynkManager::BlynkRead(Reading reading)
{
	int i = (int)reading;
	//Serial.println("sending rq: " + String(i));
	if (QueueManager::GetReadingReceivedState(i) == 0)
	{
		//Serial.println("sending rq: " + String(i));
		xQueueSend(RequestReadingQueue, (void*)&i, 0);
		QueueManager::SetReadingReceivedState(i, 1);
	}

	// pick a widget with frequency mode on the same screen as the LED widget to update it
	// this is because Blynk LEDs don't have a frequency mode
	switch (reading)
	{
		case Reading::H1:
			xQueueSend(RequestHeaterLedQueue, (void*)&i, 0);
			break;
		case Reading::HAvg1:
			xQueueSend(RequestRelayLedQueue, (void*)&i, 0);
			//Serial.print("RequestReadingQueue:"); Serial.println(uxQueueMessagesWaiting(RequestReadingQueue));
			break;
	}
}

void BlynkManager::BlynkWrite(Setting setting, int value)
{
	IndexValue iv = { (int)setting, value };
	xQueueSend(SaveSettingQueue, (void*)&iv, 0);
}

void BlynkManager::Loop()
{
	timer.run();
	if (Blynk.connected())
	{
		Blynk.run();
		if (SendReadingQueue != 0)
		{
			ReadingValue rv;
			if (xQueueReceive(SendReadingQueue, &rv, 0 ))
			{
				//Serial.println("receiving:" + String(rv.index));
				if (QueueManager::ReadingIndexIsValid(rv.index))
				{
					Reading reading = static_cast<Reading>(rv.index);
					switch (reading)
					{
					case Reading::H1:
						Blynk.virtualWrite(H1_PIN, rv.value);
						break;
					case Reading::H2:
						Blynk.virtualWrite(H2_PIN, rv.value);
						break;
					case Reading::H3:
						Blynk.virtualWrite(H3_PIN, rv.value);
						break;
					case Reading::H4:
						Blynk.virtualWrite(H4_PIN, rv.value);
						break;
					case Reading::H5:
						Blynk.virtualWrite(H5_PIN, rv.value);
						break;
					case Reading::H6:
						Blynk.virtualWrite(H6_PIN, rv.value);
						break;
					case Reading::T1:
						Blynk.virtualWrite(T1_PIN, rv.value);
						break;
					case Reading::T2:
						Blynk.virtualWrite(T2_PIN, rv.value);
						break;
					case Reading::T3:
						Blynk.virtualWrite(T3_PIN, rv.value);
						break;
					case Reading::T4:
						Blynk.virtualWrite(T4_PIN, rv.value);
						break;
					case Reading::T5:
						Blynk.virtualWrite(T5_PIN, rv.value);
						break;
					case Reading::T6:
						Blynk.virtualWrite(T6_PIN, rv.value);
						break;
					case Reading::HAvg1:
						Blynk.virtualWrite(HAVG1_VALUE_PIN, rv.value);
						Blynk.virtualWrite(HAVG1_LEVEL_PIN, rv.value);
						//Serial.print("SendReadingQueue:"); Serial.println(uxQueueMessagesWaiting(SendReadingQueue));
						break;
					case Reading::TAvg1:
						Blynk.virtualWrite(TAVG1_VALUE_PIN, rv.value);
						Blynk.virtualWrite(TAVG1_LEVEL_PIN, rv.value);
						break;
					case Reading::HAvg2:
						Blynk.virtualWrite(HAVG2_VALUE_PIN, rv.value);
						Blynk.virtualWrite(HAVG2_LEVEL_PIN, rv.value);
						break;
					case Reading::TAvg2:
						Blynk.virtualWrite(TAVG2_VALUE_PIN, rv.value);
						Blynk.virtualWrite(TAVG2_LEVEL_PIN, rv.value);
						break;
					case Reading::PAvg1:
						Blynk.virtualWrite(PAVG1_PIN, rv.value);
						break;
					case Reading::AAvg1:
						Blynk.virtualWrite(AAVG1_PIN, rv.value);
						break;
					case Reading::HChart1:
						Blynk.virtualWrite(HCHART1_PIN, rv.value);
						break;
					case Reading::TChart1:
						Blynk.virtualWrite(TCHART1_PIN, rv.value);
						break;
					case Reading::HChart2:
						Blynk.virtualWrite(HCHART2_PIN, rv.value);
						break;
					case Reading::TChart2:
						Blynk.virtualWrite(TCHART2_PIN, rv.value);
						break;
					}
					QueueManager::SetReadingReceivedState(rv.index, 0);
				}
			}
		}
		
		if (SendLedQueue != 0)
		{
			IndexValue iv;
			if (xQueueReceive(SendLedQueue, &iv, 0))
			{
				Led led = static_cast<Led>(iv.index);
				switch (led)
				{
					case Led::Heater:
						if (iv.value == 0) HeaterLED.off();
						if (iv.value == 1) HeaterLED.on();
						break;
					case Led::Relay:
						if (iv.value == 0) RelayLED.off();
						if (iv.value == 1) RelayLED.on();
						break;
				}
			}
		}

		if (SendSettingQueue != 0)
		{
			IndexValue iv;
			if (xQueueReceive(SendSettingQueue, &iv, 0))
			{
				if (QueueManager::SettingIndexIsValid(iv.index))
				{
					//Serial.println("blynk setting:" + String((int)iv.settingIndex) + ":" + String(iv.value));
					Setting setting = static_cast<Setting>(iv.index);
					switch (setting)
					{
					case Setting::SetPoint:
						Blynk.virtualWrite(SETPOINT_PIN, iv.value);
						break;
					case Setting::RelayMode:
						Blynk.virtualWrite(RELAYMODE_PIN, iv.value);
						break;
					case Setting::Hysteresis:
						Blynk.virtualWrite(HYSTERESIS_PIN, iv.value);
						break;
					case Setting::Calibrate:
						Blynk.virtualWrite(CALIBRATE_PIN, iv.value);
						break;
					case Setting::TempUnit:
						Blynk.virtualWrite(TEMPUNIT_PIN, iv.value);
						break;
					case Setting::Heater:
						Blynk.virtualWrite(HEATER_PIN, iv.value);
						break;
					}
				}
			}
		}
	}
}

void BlynkManager::checkConnection()
{
	if (!Blynk.connected() && !instance->connectInProgress)
	{
		instance->connectInProgress = true;

		if (!Blynk.connect())
		{
			Serial.println("Blynk connect failed!");
		}
		//else
		//{
		//	Serial.println("[_\\|/_] Blynk Connected");
		//}

		instance->connectInProgress = false; // either it connected or timed out
	}
}

BLYNK_CONNECTED()
{
		Blynk.syncAll();
		Blynk.notify("ESP32 Starting!");
		Serial.println("Blynk connected!");
}

BLYNK_READ(HAVG1_VALUE_PIN)
{
	BlynkManager::BlynkRead(Reading::HAvg1);
}

BLYNK_READ(TAVG1_VALUE_PIN)
{
	BlynkManager::BlynkRead(Reading::TAvg1);
}

BLYNK_READ(HAVG2_VALUE_PIN)
{
	BlynkManager::BlynkRead(Reading::HAvg2);
}

BLYNK_READ(TAVG2_VALUE_PIN)
{
	BlynkManager::BlynkRead(Reading::TAvg2);
}

BLYNK_READ(PAVG1_PIN)
{
	BlynkManager::BlynkRead(Reading::PAvg1);
}

BLYNK_READ(AAVG1_PIN)
{
	BlynkManager::BlynkRead(Reading::AAvg1);
}

BLYNK_READ(H1_PIN)
{
	BlynkManager::BlynkRead(Reading::H1);
}

BLYNK_READ(H2_PIN)
{
	BlynkManager::BlynkRead(Reading::H2);
}

BLYNK_READ(H3_PIN)
{
	BlynkManager::BlynkRead(Reading::H3);
}

BLYNK_READ(H4_PIN)
{
	BlynkManager::BlynkRead(Reading::H4);
}

BLYNK_READ(H5_PIN)
{
	BlynkManager::BlynkRead(Reading::H5);
}

BLYNK_READ(H6_PIN)
{
	BlynkManager::BlynkRead(Reading::H6);
}

BLYNK_READ(T1_PIN)
{
	BlynkManager::BlynkRead(Reading::T1);
}

BLYNK_READ(T2_PIN)
{
	BlynkManager::BlynkRead(Reading::T2);
}

BLYNK_READ(T3_PIN)
{
	BlynkManager::BlynkRead(Reading::T3);
}

BLYNK_READ(T4_PIN)
{
	BlynkManager::BlynkRead(Reading::T4);
}

BLYNK_READ(T5_PIN)
{
	BlynkManager::BlynkRead(Reading::T5);
}

BLYNK_READ(T6_PIN)
{
	BlynkManager::BlynkRead(Reading::T6);
}

BLYNK_WRITE(SETPOINT_PIN)
{
	BlynkManager::BlynkWrite(Setting::SetPoint, param.asInt());
}

BLYNK_WRITE(RELAYMODE_PIN)
{
	BlynkManager::BlynkWrite(Setting::RelayMode, param.asInt());
}

BLYNK_WRITE(HYSTERESIS_PIN)
{
	BlynkManager::BlynkWrite(Setting::Hysteresis, param.asInt());
}

BLYNK_WRITE(CALIBRATE_PIN)
{
	BlynkManager::BlynkWrite(Setting::Calibrate, param.asInt());
}

BLYNK_WRITE(TEMPUNIT_PIN)
{
	BlynkManager::BlynkWrite(Setting::TempUnit, param.asInt());
}

BLYNK_WRITE(HEATER_PIN)
{
	BlynkManager::BlynkWrite(Setting::Heater, param.asInt());
}

//BLYNK_WRITE(V2)
//{
//	//Serial.println("param.asInt:" + String(param.asInt()));
//	//digitalWrite(LED_PIN, param.asInt());
//}

BlynkManager *BlynkManager::instance;

// define Blynk here (copied from BlynkSimpleEsp32.h)
BlynkWifi Blynk(_blynkTransport);

// define all widgets here
WidgetLED RelayLED(RELAY_LED_PIN);
WidgetLED HeaterLED(HEATER_LED1_PIN);


