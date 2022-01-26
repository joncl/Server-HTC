// 
// 
// 

#include "Hysteresis.h"

void Hysteresis::Begin()
{
}

void Hysteresis::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "HYST", FontType::Condensed);

	hysteresis = Settings::GetHysteresis();
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(hysteresis), FontType::Normal);
}

void Hysteresis::Exit()
{
	Settings::SaveHysteresis(hysteresis);
}

void Hysteresis::UpButtonPress()
{
	hysteresis++;
	if (hysteresis > MAX_HYSTERESIS) hysteresis = MAX_HYSTERESIS;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(hysteresis), FontType::Normal);
}

void Hysteresis::DownButtonPress()
{
	hysteresis--;
	if (hysteresis < 1) hysteresis = 1;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(hysteresis), FontType::Normal);
}
