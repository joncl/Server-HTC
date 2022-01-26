
#ifndef _GLOBALS_h
#define _GLOBALS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <iostream>
#include <functional>
#include "ObserverManager.h"
#include "Settings.h"

// wifi and web server
const std::string HOST = "Humidity-Relay";
const std::string WIFI_SSID = "the_ssid";
const std::string WIFI_PASSWORD = "the_password";
const int WEB_SERVER_PORT = 80;
const int WIFI_TIMEOUT = 10;								// seconds

// blynk
const char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char cloudServer[] = "blynk-cloud.com";
const char localServer[] = "xxx.xxx.xxx.xxx";
const unsigned long BLYNK_CONNECTION_CHECK_INTERVAL = 60;	// seconds


// default
const int DEFAULT_SETPOINT = 50;
const int DEFAULT_RELAYMODE = 0;							// low = 0, high = 1
const int DEFAULT_HYSTERESIS = 3;
const int DEFAULT_CALIBRATION = 0;
const int DEFAULT_TEMPUNIT = 1;								// C = 0, F = 1

// global settings
const int READ_SENSOR_REALTIME_INTERVAL = 2;				// seconds
const int READ_SENSOR_CHART_INTERVAL = 30;					// seconds
const int MAX_HYSTERESIS = 10;
const int MAX_CALIBRATION = 10;
const int SENSOR_DETECT_TIMOUT = 3;							// seconds
const int TEMPUNIT_CHART = 1;								// C = 0, F = 1

// button
#define BUTTON_RESET_PIN	34								// Reset
#define BUTTON_SET_PIN		35								// Set
#define BUTTON_UP_PIN		32								// Up
#define BUTTON_DOWN_PIN		33								// Down

// sensor
#define BME_1_ADDRESS				0x77
#define BME_2_ADDRESS				0X76
#define SHT_1_ADDRESS				0x44	
#define SHT_2_ADDRESS				0x45
#define DHT_1_PIN					16
#define DHT_2_PIN					17
const int HEAT_TIME_ON = 			180;					// seconds

// display
#define LEFT_DISPLAY_ADDRESS	0x3C
#define RIGHT_DISPLAY_ADDRESS	0x3D

// led
const int HEATER_LED_PIN = 4;
const int RELAY_LED_PIN = 15;
const int RELAY_LED_LOCAL_PIN = 2;

// states
enum class State : int {
	Normal = 0,		// Show H and T info every x seconds
	Set,			// Set H relay point
	RelayMode,		// Switch above or below set H
	Hysteresis,		// Hysteresis on H
	Calibrate,		// H calibration
	TempUnit,		// Temperature unit, C or F
	Heater,			// Heater on SHT senors on or off
	Sensors,		// Read senors
	Exit,			// Exit menu
	Reset			// Reset to factory
};

// buttons
enum class ButtonName : int 
{
	Reset = 0,
	Set,
	Up,
	Down
};

#endif