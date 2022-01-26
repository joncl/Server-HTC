// 
// 
// 

#include "TempUnit.h"

void TempUnit::Begin()
{
}

void TempUnit::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "UNIT", FontType::Condensed);

	unit = Settings::GetTempUnit();
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getUnit(), FontType::Normal);
}

void TempUnit::Exit()
{
	Settings::SaveTempUnit(unit);
}

void TempUnit::UpButtonPress()
{
	doPress();
}

void TempUnit::DownButtonPress()
{
	doPress();
}

void TempUnit::doPress()
{
	unit = !unit;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getUnit(), FontType::Normal);
}

String TempUnit::getUnit()
{
	if (unit == 0) return "C";
	if (unit == 1) return "F";
}
