// 
// 
// 

#include "BmeSensor.h"

void BmeSensor::Begin(int number, uint8_t address)
{
	_number = number;

	unsigned long current = millis();
	while(1)
	{
		bool ready = bme.begin(address);
		if (ready) break;
		if (millis() - current > SENSOR_DETECT_TIMOUT * 1000)
		{
			Serial.println();
			Serial.println();
			Serial.println("Could not start BME sensor #" + String(_number) + "!");
			break;
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void BmeSensor::Begin(int number, int pin)
{
}

int BmeSensor::Number()
{
	return _number;
}

float BmeSensor::Humidity()
{
	return bme.readHumidity();
}

float BmeSensor::Temperature()
{
	return bme.readTemperature();
}

float BmeSensor::Pressure()
{
	return bme.readPressure();
}

float BmeSensor::Altitude()
{
	return bme.readAltitude(SEALEVELPRESSURE_HPA);
}
