// QueueManager.h

#ifndef _QUEUEMANAGER_h
#define _QUEUEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <array>

enum class Reading : int
{
	None = 0,	// 0
	H1,			// 1
	H2,			// 2
	H3,			// 3
	H4,			// 4
	H5,			// 5
	H6,			// 6
	T1,			// 7
	T2,			// 8
	T3,			// 9
	T4,			// 10
	T5,			// 11
	T6,			// 12
	P1,			// 13
	P2,			// 14
	A1,			// 15
	A2,			// 16
	HAvg1,		// 17
	TAvg1,		// 18
	HAvg2,		// 19
	TAvg2,		// 20
	PAvg1,		// 21
	AAvg1,		// 22
	HChart1,	// 23
	TChart1,	// 24
	HChart2,	// 25
	TChart2,	// 26
	LAST
};

enum class Setting : int
{
	SetPoint = 0,
	RelayMode,
	Hysteresis,
	Calibrate,
	TempUnit,
	Heater,
	LAST
};

enum class Led : int
{
	Heater = 0,
	Relay
};

struct ReadingValue
{
	int index;
	float value;
};

struct IndexValue
{
	int index;
	int value;
};

extern QueueHandle_t RequestReadingQueue;
extern QueueHandle_t SendReadingQueue;
extern QueueHandle_t RequestHeaterLedQueue;
extern QueueHandle_t RequestRelayLedQueue;
extern QueueHandle_t SendLedQueue;
extern QueueHandle_t SendSettingQueue;
extern QueueHandle_t SaveSettingQueue;

class QueueManager
{
public:
	static void Begin();
	static bool ReadingIndexIsValid(int index);
	static bool SettingIndexIsValid(int index);
	static void SetReadingReceivedState(int i, int v);
	static int GetReadingReceivedState(int i);

private:
	static std::array<int, 27> readingReceivedStates;
};

#endif

