// BlynkManager.h

#ifndef _BLYNKMANAGER_h
#define _BLYNKMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <map>
#include <functional>
#include "Globals.h"
#include "BlynkSimpleEsp32.h"
#include "QueueManager.h"

// comment/copy/paste these four lines from the bottom of BlynkSimpleEsp32.h
static WiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);

// only declare Blynk - define Blynk in cpp,
// otherwise you get two Blynks when including from the ino!
extern BlynkWifi Blynk;

// include widgets here
#include <BlynkWidgets.h>

// declare widgets here
extern WidgetLED RelayLED;
extern WidgetLED HeaterLED;

#define H1_PIN				V1
#define H2_PIN				V2
#define H3_PIN				V3
#define H4_PIN				V4
#define H5_PIN				V5
#define H6_PIN				V6

#define T1_PIN				V7
#define T2_PIN				V8
#define T3_PIN				V9
#define T4_PIN				V10
#define T5_PIN				V11
#define T6_PIN				V12

#define HAVG1_VALUE_PIN		V21
#define TAVG1_VALUE_PIN		V22
#define HAVG2_VALUE_PIN		V23
#define TAVG2_VALUE_PIN		V24

#define HAVG1_LEVEL_PIN		V25
#define TAVG1_LEVEL_PIN		V26
#define HAVG2_LEVEL_PIN		V27
#define TAVG2_LEVEL_PIN		V28

#define PAVG1_PIN			V29
#define AAVG1_PIN			V30

#define SETPOINT_PIN		V31
#define RELAYMODE_PIN		V32
#define HYSTERESIS_PIN		V33
#define CALIBRATE_PIN		V34
#define TEMPUNIT_PIN		V35
#define HEATER_PIN			V36

#define HCHART1_PIN			V41
#define HCHART2_PIN			V42
#define TCHART1_PIN			V43
#define TCHART2_PIN			V44

#define RELAY_LED_PIN		V51
#define HEATER_LED1_PIN		V52


class BlynkManager
{
public:
	void Begin();
	static void BlynkRead(Reading reading);
	static void BlynkWrite(Setting setting, int value);

	void Loop();

private:
	static BlynkManager *instance;
	BlynkTimer timer;
	bool connectInProgress = false;
	static void checkConnection();
};

#endif

