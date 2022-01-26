// WebServer.h

#ifndef _WEBSERVER_h
#define _WEBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "esp_wps.h"
#include "Globals.h"

class WebServer
{
public:
	void Begin();
	static AsyncWebServer* Server();

private:
	static AsyncWebServer server;
	static const char home_content[];
};

#endif

