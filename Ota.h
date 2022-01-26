// Ota.h

#ifndef _OTA_h
#define _OTA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "WebServer.h"

class Ota
{
public:
	void Begin();

private:
	static const char ota_content[];
};

#endif

