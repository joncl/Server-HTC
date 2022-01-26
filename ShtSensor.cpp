// 
// 
// 

#include "ShtSensor.h"

void ShtSensor::Begin(int number, uint8_t address)
{
	_number = number;

	unsigned long current = millis();
	while (1)
	{
		bool ready = sht.begin(address);
		if (ready) break;
		if (millis() - current > SENSOR_DETECT_TIMOUT * 1000)
		{
			Serial.println();
			Serial.println();
			Serial.println("Could not start SHT sensor #" + String(_number) + "!");
			break;
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void ShtSensor::Begin(int number, int pin)
{
}

int ShtSensor::Number()
{
	return _number;
}

float ShtSensor::Humidity()
{
	return sht.readHumidity();
}

float ShtSensor::Temperature()
{
	return sht.readTemperature();
}

float ShtSensor::Pressure()
{
	return 0.0f;
}

float ShtSensor::Altitude()
{
	return 0.0f;
}

void ShtSensor::HeaterState(int h)
{
	sht.heater(h);
}
