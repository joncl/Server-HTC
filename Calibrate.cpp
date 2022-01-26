// 
// 
// 

#include "Calibrate.h"

void Calibrate::Begin()
{
}

void Calibrate::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "CALH", FontType::Condensed);

	calibration = Settings::GetCalibration();
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(calibration), FontType::Normal);
}

void Calibrate::Exit()
{
	Settings::SaveCalibration(calibration);
}

void Calibrate::UpButtonPress()
{

	calibration++;
	if (calibration > MAX_CALIBRATION) calibration = MAX_CALIBRATION;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(calibration), FontType::Normal);
}

void Calibrate::DownButtonPress()
{
	calibration--;
	if (calibration < -MAX_CALIBRATION) calibration = -MAX_CALIBRATION;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(calibration), FontType::Normal);
}

