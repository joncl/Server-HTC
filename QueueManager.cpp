// 
// 
// 

#include "QueueManager.h"

void QueueManager::Begin()
{
	//Serial.println("sizeof:" + String(sizeof(ReadingValue)));

	RequestReadingQueue = xQueueCreate(64, sizeof(int));
	SendReadingQueue = xQueueCreate(64, sizeof(ReadingValue));
	RequestHeaterLedQueue = xQueueCreate(4, sizeof(int));
	RequestRelayLedQueue = xQueueCreate(4, sizeof(int));
	SendLedQueue = xQueueCreate(4, sizeof(IndexValue));
	SendSettingQueue = xQueueCreate(16, sizeof(IndexValue));
	SaveSettingQueue = xQueueCreate(16, sizeof(IndexValue));

}

bool QueueManager::ReadingIndexIsValid(int index)
{
	return (index >= 1 && index < (int)Reading::LAST);
}

bool QueueManager::SettingIndexIsValid(int index)
{
	return (index >= 0 && index < (int)Setting::LAST);
}

void QueueManager::SetReadingReceivedState(int i, int v)
{
	readingReceivedStates[i] = v;
}

int QueueManager::GetReadingReceivedState(int i)
{
	return readingReceivedStates[i];
}

QueueHandle_t RequestReadingQueue;
QueueHandle_t SendReadingQueue;
QueueHandle_t RequestHeaterLedQueue;
QueueHandle_t RequestRelayLedQueue;
QueueHandle_t SendLedQueue;
QueueHandle_t SendSettingQueue;
QueueHandle_t SaveSettingQueue;

std::array<int, 27> QueueManager::readingReceivedStates = { 0 };