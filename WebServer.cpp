// 
// 
// 

#include "WebServer.h"

void WebServer::Begin()
{
	Serial.println("webserver begin, core:" + String(xPortGetCoreID()));

	/*use mdns for host name resolution*/
	if (!MDNS.begin(HOST.c_str())) {
		Serial.println("Error setting up MDNS responder!");
		while (1) {
			delay(1000);
		}
	}
	Serial.println("mDNS responder started");

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", WebServer::home_content);
	});

	WebServer::server.begin();
}

AsyncWebServer* WebServer::Server()
{
	return &server;
}

AsyncWebServer WebServer::server(WEB_SERVER_PORT);

const char WebServer::home_content[] PROGMEM = {
	"<html>\r\n"
	"<head>\r\n"
	"<meta charset='utf-8'>\r\n"
	"<title>Humidity-Relay</title>\r\n"
	"</head>\r\n"
	"<body style='font-family=\"verdana\"'>\r\n"
	"<div><h3>Humidity-Relay</h3></div>\r\n"
	"<a href='/ota'>Update firmware</a>\r\n"
	"</body>\r\n"
	"</html>\r\n"
};
