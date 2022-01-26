// 
// 
// 

#include "RelayMode.h"

void RelayMode::Begin()
{
}

void RelayMode::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "RELAY", FontType::ExtraCondensed);

	relayMode = Settings::GetRelayMode();
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getOutput(), FontType::Condensed);
}

void RelayMode::Exit()
{
	Settings::SaveRelayMode(relayMode);
}

void RelayMode::UpButtonPress()
{
	relayMode = !relayMode;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getOutput(), FontType::Condensed);
}

void RelayMode::DownButtonPress()
{
	relayMode = !relayMode;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getOutput(), FontType::Condensed);
}

String RelayMode::getOutput()
{
	if (relayMode == 0) return "LOW";
	if (relayMode == 1) return "HIGH";
}
