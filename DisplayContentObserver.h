// DisplayContentObserver

#ifndef _DISPLAYCONTENTOBSERVER_h
#define _DISPLAYCONTENTOBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <functional>
#include "Globals.h"

enum class FontType : int { Normal = 0, Condensed, ExtraCondensed, ExtraCondensedShrunk };

struct DisplayInfo {
	String Name;
	//std::function<float()> ReadPointer;
	Reading reading;
	FontType NameFontType;
	FontType ValueFontType;
};

class DisplayContentObserver : public ObserverBase
{
public:
	virtual void LeftDisplay(String s, FontType f) = 0;
	virtual void RightDisplay(String s, FontType f) = 0;
};

#endif