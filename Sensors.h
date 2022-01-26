// Sensors.h

#ifndef _SENSOR_h
#define _SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#endif

#include "IState.h"
#include "ISensor.h"
#include "BmeSensor.h"
#include "ShtSensor.h"
#include "DhtSensor.h"
#include "DisplayContentObserver.h"
#include "SensorDataObserver.h"
#include "QueueManager.h"

typedef std::function<float()> SensorPointer;
typedef std::map<Reading, SensorPointer> SensorPointers;

enum class ReadingType : int
{
	Humidity = 0,
	Temperature,
	Pressure,
	Altitude
};

class Sensors : public IState
{
public:
	// Inherited via IState
	virtual void Begin() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void UpButtonPress() override;
	virtual void DownButtonPress() override;

	void Loop();

private:
	int current;
	unsigned long counterForRealtime;
	unsigned long counterForChart;
	BmeSensor bme1;
	BmeSensor bme2;
	ShtSensor sht1;
	ShtSensor sht2;
	DhtSensor dht1;
	DhtSensor dht2;
	SensorPointers pointers;
	std::vector<ISensor*> sensors;
	std::vector<DisplayInfo> menu;
	bool menuActive = false;
	static const Matrix<int, 23, 6> pointerMap;
	static const Matrix<int, 23, 3> menuMap;

	void mapPointers();
	float read(Reading reading, bool isForChart=false);
	void notifyDisplays();
	void notifyLeftDisplay();
	void notifyRightDisplay();
	void buildMenu();
};

#endif
