// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Globals.h"
#include "Font.h"
#include "DisplayContentObserver.h"

class Display : public DisplayContentObserver
{
public:
	void Begin();

	// Inherited via DisplayStateObserver
	virtual void LeftDisplay(String s, FontType f) override;
	virtual void RightDisplay(String s, FontType f) override;

private:
	Adafruit_SSD1306 *left;
	Adafruit_SSD1306 *right;
	void updateDisplay(Adafruit_SSD1306 *display, String content, FontType fontType);
	const GFXfont *fontNormal = &SairaSemiCondensed_Regular74pt7b;
	const GFXfont *fontCondensed = &SairaCondensed_Regular74pt7b;
	const GFXfont *fontExtraCondensed = &SairaExtraCondensed_Regular74pt7b;
	const GFXfont *fontExtraCondensedShrunk = &SairaExtraCondensed_Regular54pt7b;
};

#endif
