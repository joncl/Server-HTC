// 
// 
// 

#include "Display.h"

void Display::Begin()
{
	//Serial.println("display begin, core:" + String(xPortGetCoreID()));
	left = new Adafruit_SSD1306();
	left->begin(SSD1306_SWITCHCAPVCC, 0x3C);

	right = new Adafruit_SSD1306();
	right->begin(SSD1306_SWITCHCAPVCC, 0x3D);

	ObserverManager::subscribe<DisplayContentObserver>(this);
}

void Display::LeftDisplay(String s, FontType f)
{
	updateDisplay(left, s, f);
}

void Display::RightDisplay(String s, FontType f)
{
	updateDisplay(right, s, f);
}

void Display::updateDisplay(Adafruit_SSD1306 *display, String content, FontType fontType)
{
	switch (fontType)
	{
	case FontType::Normal:
		display->setFont(fontNormal);
		break;
	case FontType::Condensed:
		display->setFont(fontCondensed);
		break;
	case FontType::ExtraCondensed:
		display->setFont(fontExtraCondensed);
		break;
	case FontType::ExtraCondensedShrunk:
		display->setFont(fontExtraCondensedShrunk);
	}

	display->clearDisplay();
	display->setTextSize(2);
	display->setTextColor(WHITE);
	display->setCursor(0, 54);
	display->println(content);
	display->display();
}
