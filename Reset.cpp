// 
// 
// 

#include "Reset.h"

void Reset::Begin()
{
}

void Reset::Enter()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay, "RESET?", FontType::Condensed);
	reset = true;
	doPress();
}

void Reset::Exit()
{
	if (reset) Settings::Reset();
}

void Reset::UpButtonPress()
{
	doPress();
}

void Reset::DownButtonPress()
{
	doPress();
}

void Reset::doPress()
{
	reset = !reset;
	ObserverManager::notify(&DisplayContentObserver::RightDisplay, getAnswer(), FontType::Normal);
}

String Reset::getAnswer()
{
	if (!reset) return "N";
	if (reset) return "Y";
}



