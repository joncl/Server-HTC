//
//
//

#include "ExitSettings.h"

void ExitSettings::Begin()
{
}

void ExitSettings::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "EXIT?", FontType::Condensed);
	input = true;
	doPress();
}

void ExitSettings::Exit()
{
}

void ExitSettings::UpButtonPress()
{
	doPress();
}

void ExitSettings::DownButtonPress()
{
	doPress();
}

void ExitSettings::doPress()
{
	input = !input;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getAnswer(), FontType::Normal);
	ObserverManager::notify(&ExitStateObserver::SettingsExit, input);
}

String ExitSettings::getAnswer()
{
	if (!input) return "N";
	if (input) return "Y";
}
