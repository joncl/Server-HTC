#include <cstddef>
#include <WiFi.h>
#include <Wire.h>
#include "Globals.h"
#include "ButtonManager.h"
#include "StateManager.h"
#include "BlynkSimpleEsp32.h"
#include "BlynkManager.h"
#include "Relay.h"
#include "WebServer.h"
#include "Ota.h"

//#define INCLUDE_uxTaskGetStackHighWaterMark     1
//UBaseType_t uxHighWaterMark;

TaskHandle_t TaskHandle1, TaskHandle2;

bool connecting = true;

void setup() {
	while (!Serial);
	delay(1000);
	Serial.begin(115200);

	Wire.begin();
	delay(100);					// delay for stability

	QueueManager::Begin();

	xTaskCreatePinnedToCore(
		Task1,					// pvTaskCode
		"Workload1",			// pcName
		10240,					// usStackDepth
		NULL,					// pvParameters
		1,						// uxPriority
		&TaskHandle1,			// pxCreatedTask
		1);						// xCoreID

	//// start task3 after connection attempted.
	//while (connecting) { delay(250); }

	xTaskCreatePinnedToCore(
		Task2,					// pvTaskCode
		"Workload2",			// pcName
		10240,					// usStackDepth
		NULL,					// pvParameters
		1,						// uxPriority
		&TaskHandle2,			// pxCreatedTask
		0);						// xCoreID
}

void loop() {
	//delay(portMAX_DELAY);
	delay(2000);
}

void Task1(void * parameter)
{
	//uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
	//Serial.println("task1 stack remaining:");
	//Serial.print(uxHighWaterMark);
	//Serial.println();

	ButtonManager buttonManager;
	StateManager stateManager;

	buttonManager.Begin();
	ObserverManager::subscribe<StateManagerObserver>(&buttonManager);

	stateManager.Begin();
	ObserverManager::subscribe<ButtonManagerObserver>(&stateManager);
	ObserverManager::subscribe<ExitStateObserver>(&stateManager);

	Relay relay;
	relay.Begin();

	unsigned long counter;

	for (;;)
	{
		buttonManager.Loop();
		stateManager.Loop();
		// Adafruit_SHT13 library has a taskdelay of 15, so no vTaskDelay needed here:
		//vTaskDelay(10 / portTICK_PERIOD_MS);
		unsigned long newMillis = millis();
		if ((newMillis - counter) >= 1000)
		{
			counter = millis();
			//Serial.println("stack remaining:" + String(uxHighWaterMark));
		}
	}
	vTaskDelete(NULL);
}

void Task2(void * parameter)
{
	//uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
	//Serial.println("task2 stack remaining:");
	//Serial.print(uxHighWaterMark);
	//Serial.println();

	WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
	Serial.println();
	Serial.print("Connecting to WiFi");
	unsigned long current = millis();
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
		if (millis() - current > WIFI_TIMEOUT * 1000)
		{
			Serial.println();
			Serial.println();
			Serial.println("Could not connect to WIFI!");
			break;
		}
	}
	connecting = false;
	Serial.println();

	Serial.print("IP:");
	Serial.print(WiFi.localIP());
	Serial.println();
	Serial.println();

	WebServer webServer;

	Ota ota;
	ota.Begin();

	webServer.Begin();

	BlynkManager blynkManager;
	blynkManager.Begin();

	for (;;)
	{
		blynkManager.Loop();
		vTaskDelay(10 / portTICK_PERIOD_MS);
		//Serial.println("stack remaining:" + String(uxHighWaterMark));
	}
	vTaskDelete(NULL);
}
