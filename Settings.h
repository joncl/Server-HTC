// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <Preferences.h>
#include <iostream>
#include <vector>
#include <map>
#include "Globals.h"
#include "QueueManager.h"
#include "HeaterObserver.h"
#include "HeaterSettingObserver.h"

class Settings : public HeaterObserver
{
public:
	static void Begin();

	static int GetSetPoint();
	static void SaveSetPoint(int v);

	static int GetRelayMode();
	static void SaveRelayMode(int v);

	static int GetHysteresis();
	static void SaveHysteresis(int v);

	static int GetCalibration();
	static void SaveCalibration(int v);

	static int GetTempUnit();
	static void SaveTempUnit(int v);

	static String GetTempUnitString();

	static void Reset();

	static std::vector<int> GetThresholds();

	static void Loop();

	// Inherited via HeaterObserver
	virtual void HeaterState(int h) override;

private:
	static Settings instance;
	Preferences preferences;
	std::map<Setting, const char*> keyMap;
	const char* settingsKey = "settings";

	int setPoint = 0;
	int relayMode = 0;
	int hysteresis = 0;
	int calibration = 0;
	int tempUnit = 0;

	std::vector<int> thresholds = { 0, 0 };

	//void initKeyMap();

	int getSetPoint();
	void saveSetPoint(int v, bool sendToQueue=true);
	
	int getRelayMode();
	void saveRelayMode(int v, bool sendToQueue = true);
	
	int getHysteresis();
	void saveHysteresis(int v, bool sendToQueue = true);

	int getCalibration();
	void saveCalibration(int v, bool sendToQueue = true);
	
	int getTempUnit();
	void saveTempUnit(int v, bool sendToQueue = true);

	void reset();

	void populate();

	int read(Setting s, bool queue=false);
	void save(Setting s, int v, bool queue);

	void updateThresholds();

	void queueSend(Setting s, int v);
};

#endif

