// 
// 
// 

#include "Settings.h"

void Settings::Begin()
{
	instance.keyMap[Setting::SetPoint] = "set_point";
	instance.keyMap[Setting::RelayMode] = "relay_mode";
	instance.keyMap[Setting::Hysteresis] = "hysteresis";
	instance.keyMap[Setting::Calibrate] = "calibration";
	instance.keyMap[Setting::TempUnit] = "temp_unit";

	// if setPoint is 0, reset to default settings
	if (instance.read(Setting::SetPoint) == 0)
	{
		instance.reset();
	}
	else
	{
		instance.populate();
	}
	instance.updateThresholds();

	ObserverManager::subscribe<HeaterObserver>(&instance);
}

int Settings::GetSetPoint()
{
	return instance.setPoint;
}

void Settings::SaveSetPoint(int v)
{
	instance.saveSetPoint(v);
}

int Settings::GetRelayMode()
{
	return instance.relayMode;
}

void Settings::SaveRelayMode(int v)
{
	instance.saveRelayMode(v);
}

int Settings::GetHysteresis()
{
	return instance.hysteresis;
}

void Settings::SaveHysteresis(int v)
{
	instance.saveHysteresis(v);
}

int Settings::GetCalibration()
{
	return instance.calibration;
}

void Settings::SaveCalibration(int v)
{
	instance.saveCalibration(v);
}

int Settings::GetTempUnit()
{
	return instance.tempUnit;
}

void Settings::SaveTempUnit(int v)
{
	instance.saveTempUnit(v);
}

String Settings::GetTempUnitString()
{
	return instance.tempUnit == 0 ? "C" : "F";
}

void Settings::Reset()
{
	instance.reset();
}

std::vector<int> Settings::GetThresholds()
{
	return instance.thresholds;
}

void Settings::Loop()
{
	IndexValue iv;
	if (xQueueReceive(SaveSettingQueue, &iv, 0))
	{
		if (QueueManager::SettingIndexIsValid(iv.index))
		{
			//Serial.println("setting:" + String((int)sv.settingIndex) + ":" + String(sv.value));
			Setting setting = static_cast<Setting>(iv.index);
			switch (setting)
			{
			case Setting::SetPoint:
				instance.saveSetPoint(iv.value, false);
				break;
			case Setting::RelayMode:
				instance.saveRelayMode(iv.value, false);
				break;
			case Setting::Hysteresis:
				instance.saveHysteresis(iv.value, false);
				break;
			case Setting::Calibrate:
				instance.saveCalibration(iv.value, false);
				break;
			case Setting::TempUnit:
				instance.saveTempUnit(iv.value, false);
				break;
			case Setting::Heater:
				ObserverManager::notify(&HeaterSettingObserver::HeaterState, iv.value);
				break;
			}
		}
	}
}

void Settings::HeaterState(int h)
{
	IndexValue iv = { (int)Setting::Heater, h};
	xQueueSend(SendSettingQueue, (void*)&iv, 0);
}

// private members

int Settings::getSetPoint()
{
	return setPoint;
}

void Settings::saveSetPoint(int v, bool sendToQueue)
{
	if (setPoint == v) return;
	setPoint = v;
	save(Setting::SetPoint, v, sendToQueue);
	updateThresholds();
}

int Settings::getRelayMode()
{
	return relayMode;
}

void Settings::saveRelayMode(int v, bool sendToQueue)
{
	if (relayMode == v) return;
	relayMode = v;
	save(Setting::RelayMode, v, sendToQueue);
}

int Settings::getHysteresis()
{
	return hysteresis;
}

void Settings::saveHysteresis(int v, bool sendToQueue)
{
	if (hysteresis == v) return;
	hysteresis = v;
	save(Setting::Hysteresis, v, sendToQueue);
	updateThresholds();
}

int Settings::getCalibration()
{
	return calibration;
}

void Settings::saveCalibration(int v, bool sendToQueue)
{
	if (calibration == v) return;
	calibration = v;
	save(Setting::Calibrate, v, sendToQueue);
}

int Settings::getTempUnit()
{
	return tempUnit;
}

void Settings::saveTempUnit(int v, bool sendToQueue)
{
	if (tempUnit == v) return;
	tempUnit = v;
	save(Setting::TempUnit, v, sendToQueue);
}

void Settings::reset()
{
	saveSetPoint(DEFAULT_SETPOINT);
	saveRelayMode(DEFAULT_RELAYMODE);
	saveHysteresis(DEFAULT_HYSTERESIS);
	saveCalibration(DEFAULT_CALIBRATION);
	saveTempUnit(DEFAULT_TEMPUNIT);
	updateThresholds();
}

void Settings::populate()
{
	setPoint = read(Setting::SetPoint, true);
	relayMode = read(Setting::RelayMode, true);
	hysteresis = read(Setting::Hysteresis, true);
	calibration = read(Setting::Calibrate, true);
	tempUnit = read(Setting::TempUnit, true);
}

int Settings::read(Setting s, bool sendToQueue)
{
	preferences.begin(settingsKey);
	int v = preferences.getInt(keyMap[s]);
	preferences.end();
	if (sendToQueue) queueSend(s, v);
	return v;
}

void Settings::save(Setting s, int v, bool sendToQueue)
{
	preferences.begin(settingsKey);
	preferences.putInt(keyMap[s], v);
	preferences.end();
	if (sendToQueue) queueSend(s, v);
}

void Settings::updateThresholds()
{
	switch (relayMode)
	{
	case 0:
		thresholds[0] = setPoint;
		thresholds[1] = setPoint + hysteresis;
		break;

	case 1:
		thresholds[0] = setPoint;
		thresholds[1] = setPoint - hysteresis;
		break;
	}
}

void Settings::queueSend(Setting s, int v)
{
	IndexValue iv = { (int)s, v };

	xQueueSend(SendSettingQueue, (void*)&iv, 0);
}

Settings Settings::instance;