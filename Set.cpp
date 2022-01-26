// 
// 
// 

#include "Set.h"

void Set::Begin()
{
}

void Set::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "SET", FontType::Normal);

	setPoint = Settings::GetSetPoint();
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(setPoint), FontType::Normal);
}

void Set::Exit()
{
	Settings::SaveSetPoint(setPoint);
}

void Set::UpButtonPress()
{
	setPoint++;
	if (setPoint > 100) setPoint = 100;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(setPoint), FontType::Normal);
}

void Set::DownButtonPress()
{
	setPoint--;
	if (setPoint < 1) setPoint = 1;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(setPoint), FontType::Normal);
}

