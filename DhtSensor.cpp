// 
// 
// 

#include "DhtSensor.h"

void DhtSensor::Begin(int number, uint8_t address)
{
}

void DhtSensor::Begin(int number, int pin)
{
	_number = number;
	dht.setup(pin, DHTesp::DHT22);
}

int DhtSensor::Number()
{
	return _number;
}

float DhtSensor::Humidity()
{
	return dht.getHumidity();
}

float DhtSensor::Temperature()
{
	return dht.getTemperature();
}

float DhtSensor::Pressure()
{
	return 0.0f;
}

float DhtSensor::Altitude()
{
	return 0.0f;
}

