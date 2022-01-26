// 
// 
// 

#include "Sensors.h"

void Sensors::Begin()
{
	//Serial.println("sensor begin, core:" + String(xPortGetCoreID()));

	bme1.Begin(1, (uint8_t)BME_1_ADDRESS);
	bme2.Begin(2, (uint8_t)BME_2_ADDRESS);
	sht1.Begin(3, (uint8_t)SHT_1_ADDRESS);
	sht2.Begin(4, (uint8_t)SHT_2_ADDRESS);
	dht1.Begin(5, DHT_1_PIN);
	dht2.Begin(6, DHT_2_PIN);

	sensors.push_back(&bme1);
	sensors.push_back(&bme2);
	sensors.push_back(&sht1);
	sensors.push_back(&sht2);
	sensors.push_back(&dht1);
	sensors.push_back(&dht2);

	mapPointers();
	buildMenu();
	counterForRealtime = millis();
}

void Sensors::Enter()
{
	// note current is base0 and pointerMap/menuMap are base1 !!
	current = 0;
	menuActive = true;
	notifyDisplays();
}

void Sensors::Exit()
{
	menuActive = false;
}

void Sensors::UpButtonPress()
{
	current--;
	if (current < 0) current = menu.size() - 1;
	notifyDisplays();
}

void Sensors::DownButtonPress()
{
	current++;
	if (current > (menu.size() - 1)) current = 0;
	notifyDisplays();
}

void Sensors::Loop()
{
	// reading for display (realtime)
	unsigned long newMillis = millis();
	if ((newMillis - counterForRealtime) >= READ_SENSOR_REALTIME_INTERVAL * 1000)
	{
		counterForRealtime = newMillis;
		if (menuActive)
		{
			notifyRightDisplay();
		}
		else
		{
			float h = read(Reading::HAvg1);
			float t = read(Reading::TAvg1);
			
			// notify relay
			ObserverManager::notify(&SensorDataObserver::SensorData, h, t);

			// notify display
			ObserverManager::notify(&DisplayContentObserver::LeftDisplay, String(h, 1), FontType::Normal);
			ObserverManager::notify(&DisplayContentObserver::RightDisplay, String(t, 1), FontType::Normal);
		}
	}

	// readings for queue (realtime)
	int readingIndex;
	if (xQueueReceive(RequestReadingQueue, &readingIndex, 0))
	{
		// Serial.println("received request: " + String(readingIndex));
		if (QueueManager::ReadingIndexIsValid(readingIndex))
		{
			ReadingValue rv = { readingIndex, read(static_cast<Reading>(readingIndex))};
			// Serial.println("sending:" + String(rv.readingIndex) + "," + String(rv.value));
			xQueueSend(SendReadingQueue, (void*)&rv, 0);
		}
	}

	// readings for queue (chart)
	newMillis = millis();
	if ((newMillis - counterForChart) >= READ_SENSOR_CHART_INTERVAL * 1000)
	{
		counterForChart = millis();
		ReadingValue hChart1 = { (int)Reading::HChart1, read(Reading::HAvg1) };
		xQueueSend(SendReadingQueue, (void*)&hChart1, 0);
		ReadingValue tChart1 = { (int)Reading::TChart1, read(Reading::TAvg1, true) };
		xQueueSend(SendReadingQueue, (void*)&tChart1, 0);
		ReadingValue hChart2 = { (int)Reading::HChart2, read(Reading::HAvg2) };
		xQueueSend(SendReadingQueue, (void*)&hChart2, 0);
		ReadingValue tChart2 = { (int)Reading::TChart2, read(Reading::TAvg2, true) };
		xQueueSend(SendReadingQueue, (void*)&tChart2, 0);
	}
}

void Sensors::mapPointers()
{
	for (ISensor *s : sensors)
	{
		switch (s->Number())
		{
		case 1:
			pointers[Reading::H1] = std::bind(&ISensor::Humidity, s);
			pointers[Reading::T1] = std::bind(&ISensor::Temperature, s);
			pointers[Reading::P1] = std::bind(&ISensor::Pressure, s);
			pointers[Reading::A1] = std::bind(&ISensor::Altitude, s);
			break;
		case 2:
			pointers[Reading::H2] = std::bind(&ISensor::Humidity, s);
			pointers[Reading::T2] = std::bind(&ISensor::Temperature, s);
			pointers[Reading::P2] = std::bind(&ISensor::Pressure, s);
			pointers[Reading::A2] = std::bind(&ISensor::Altitude, s);
			break;
		case 3:
			pointers[Reading::H3] = std::bind(&ISensor::Humidity, s);
			pointers[Reading::T3] = std::bind(&ISensor::Temperature, s);
			break;
		case 4:
			pointers[Reading::H4] = std::bind(&ISensor::Humidity, s);
			pointers[Reading::T4] = std::bind(&ISensor::Temperature, s);
			break;
		case 5:
			pointers[Reading::H5] = std::bind(&ISensor::Humidity, s);
			pointers[Reading::T5] = std::bind(&ISensor::Temperature, s);
			break;
		case 6:
			pointers[Reading::H6] = std::bind(&ISensor::Humidity, s);
			pointers[Reading::T6] = std::bind(&ISensor::Temperature, s);
			break;
		}
	}
}

float Sensors::read(Reading reading, bool isForChart)
{
	float value = 0;
	int count = 0;

	// loop through each sensor for this sensor listed in pointerMap
	// ri is reading index!
	 //Serial.println("reading:" + String((int)reading));
	for (const int &ri : pointerMap[(int)reading])
	{
		 //Serial.println("ri:" + String(ri));
		if (ri > 0)
		{
			// get function pointer from pointers vector
			std::function<float()> ptr = pointers[static_cast<Reading>(ri)];
			if (ptr != NULL)
			{
				float tmpValue = ptr();
				if (!isnan(tmpValue) && tmpValue != 0)
				{
					// convert temperature if: (given that default temperature unit from sensors is C)
					//     1) this is a temperature reading && TempUnit settings is F (1)
					//     or
					//     2) this reading is for the chart (so will be temperature) and TEMPUNIT_CHART is F (1)
					//Serial.println("tmpValue:" + String(tmpValue) + ":" +  String(menuMap[(int)reading][0]) + ":" + String(Settings::GetTempUnit()) );
					if (
						(menuMap[(int)reading][0] == (int)ReadingType::Temperature && Settings::GetTempUnit() == 1 )
						||
						isForChart && TEMPUNIT_CHART == 1
						)
					{
						tmpValue = tmpValue * 1.8 + 32;
					}
					value = value + tmpValue;
					count++;
				}
			}
		}
	}
	if (count > 1)
	{
		// found more than one sensor for this reading
		// this is an average reading type
		value = value / count;

		// apply calibration only on average humidity
		if (static_cast<ReadingType>(menuMap[(int)reading][0]) == ReadingType::Humidity)
		{
			value = value + Settings::GetCalibration();
		}
	}
	return value;
}

void Sensors::notifyDisplays()
{
	notifyLeftDisplay();
	notifyRightDisplay();
}

void Sensors::notifyLeftDisplay()
{
	ObserverManager::notify(&DisplayContentObserver::LeftDisplay,
		menu.at(current).Name,
		menu.at(current).NameFontType);
}

void Sensors::notifyRightDisplay()
{
	ObserverManager::notify(&DisplayContentObserver::RightDisplay,
		//String(menu.at(current).ReadPointer(), 1),
		// need to shift current menu index up 1
		// because current is base0 and pointerMap/menuMap are base1 !!
		String(read(menu.at(current).reading), 1),
		menu.at(current).ValueFontType);
}

void Sensors::buildMenu()
{
	// i is Reading index, which is also both menuMap and pointerMap index
	// throw out index 0 of menuMap to align with pointerMap (pointerMap[0] is None)
	// the last two items in menuMap are averages not included in the menu
	for (std::size_t i = 1; i < menuMap.size()-6; ++i)
	{
		std::array<int, 6> sensorIndexes = pointerMap[ i ];
		
		int count = 0;
		// si is sensors index
		for (std::size_t si = 0; si < sensorIndexes.size(); ++si)
		{
			// only add menu item if listed in pointerMap (IOW is not zero)
			if (sensorIndexes[si] > 0)
			{
				switch (static_cast<ReadingType>(menuMap[i][0]))
				{
				case ReadingType::Humidity:
					menu.push_back(
						{
							"H" + String(sensors[si]->Number()) + "-%",
							static_cast<Reading>(i),
							static_cast<FontType>(menuMap[i][1]),
							static_cast<FontType>(menuMap[i][2])
						}
					);
					break;

				case ReadingType::Temperature:
					menu.push_back(
						{
							"T" + String(sensors[si]->Number()) + "-" + Settings::GetTempUnitString(),
							static_cast<Reading>(i),
							static_cast<FontType>(menuMap[i][1]),
							static_cast<FontType>(menuMap[i][2])
						}
					);
					break;

				case ReadingType::Pressure:
					menu.push_back(
						{
							"P" + String(sensors[si]->Number()) + "hPa",
							static_cast<Reading>(i),
							static_cast<FontType>(menuMap[i][1]),
							static_cast<FontType>(menuMap[i][2])
						}
					);
					break;

				case ReadingType::Altitude:
					menu.push_back(
						{
							"A" + String(sensors[si]->Number()) + "-m" ,
							static_cast<Reading>(i),
							static_cast<FontType>(menuMap[i][1]),
							static_cast<FontType>(menuMap[i][2])
						}
					);
					break;
				}
			}
		}
	}
}

//const int Sensors::pointerMap[31][6] =
//{
//	{	0,	0,	0,	0,	0,	0	},
//	{	1,	0,	0,	0,	0,	0	},
//	{	0,	2,	0,	0,	0,	0	},
//	{	0,	0,	3,	0,	0,	0	},
//	{	0,	0,	0,	4,	0,	0	},
//	{	0,	0,	0,	0,	5,	0	},
//	{	0,	0,	0,	0,	0,	6	},
//	{	7,	0,	0,	0,	0,	0	},
//	{	0,	8,	0,	0,	0,	0	},
//	{	0,	0,	9,	0,	0,	0	},
//	{	0,	0,	0,	10,	0,	0	},
//	{	0,	0,	0,	0,	11,	0	},
//	{	0,	0,	0,	0,	0,	12	},
//	{	13,	0,	0,	0,	0,	0	},
//	{	0,	14,	0,	0,	0,	0	},
//	{	19,	0,	0,	0,	0,	0	},
//	{	0,	20,	0,	0,	0,	0	},
//	{	1,	2,	3,	4,	0,	0	},
//	{	0,	0,	0,	0,	5,	6	},
//	{	7,	8,	9,	10,	0,	0	},
//	{	0,	0,	0,	0,	11,	12	},
//	{	13,	14,	0,	0,	0,	0	},
//	{	15,	16,	0,	0,	0,	0	},
//};

// const Matrix<int, 31, 3> Sensors::menuMap =
// { {
// 	// [0] maps ReadingType:
// 	//		Humidity:		0
// 	//		Temperature:	1
// 	//		Pressure:		2
// 	//		Altitue:		3
// 	// [1] maps FontType index for left display
// 	// [2] maps FontType index for right display

// 	{ 0, 0,	0 },	
// 	{ 0, 1,	0 },
// 	{ 0, 1,	0 },
// 	{ 0, 1,	0 },
// 	{ 0, 1,	0 },
// 	{ 0, 1,	0 },
// 	{ 0, 1,	0 },
// 	{ 1, 1,	0 },
// 	{ 1, 1,	0 },
// 	{ 1, 1,	0 },
// 	{ 1, 1,	0 },
// 	{ 1, 1,	0 },
// 	{ 1, 1,	0 },
// 	{ 2, 2,	2 },
// 	{ 2, 2,	2 },
// 	{ 3, 1,	1 },
// 	{ 3, 1,	1 },
// 	{ 0, 0,	0 },
// 	{ 0, 0,	0 },
// 	{ 1, 0,	0 },
// 	{ 1, 0,	0 },
// 	{ 2, 0,	0 },
// 	{ 3, 0,	0 },
// } };

const Matrix<int, 23, 6> Sensors::pointerMap =
//const int Sensors::pointerMap[29][6] =
{ {//BME1 BME2 SHT1 SHT2 DHT1 DHT2
	{  0,   0,   0,   0,   0,   0  },	// 0	None
	{  1,   0,   0,   0,   0,   0  },	// 1	H1
	{  0,   2,   0,   0,   0,   0  },	// 2	H2
	{  0,   0,   3,   0,   0,   0  },	// 3	H3
	{  0,   0,   0,   4,   0,   0  },	// 4	H4
	{  0,   0,   0,   0,   5,   0  },	// 5	H5
	{  0,   0,   0,   0,   0,   6  },	// 6	H6
	{  7,   0,   0,   0,   0,   0  },	// 7	T1
	{  0,   8,   0,   0,   0,   0  },	// 8	T2
	{  0,   0,   9,   0,   0,   0  },	// 9	T3
	{  0,   0,   0,  10,   0,   0  },	// 10	T4
	{  0,   0,   0,   0,  11,   0  },	// 11	T5
	{  0,   0,   0,   0,   0,  12  },	// 12	T6
	{ 13,   0,   0,   0,   0,   0  },	// 13	P1
	{  0,  14,   0,   0,   0,   0  },	// 14	P2
	{ 15,   0,   0,   0,   0,   0  },	// 15	A1
	{  0,  16,   0,   0,   0,   0  },	// 16	A2
	{  1,   2,   3,   4,   0,   0  },	// 17	HAvg1
	{  7,   8,   8,  10,   0,   0  },	// 18	TAvg1
	{  0,   0,   0,   0,   5,   6  },	// 19	HAvg2
	{  0,   0,   0,   0,  11,  12  },	// 20	TAvg2
	{ 13,  14,   0,   0,   0,   0  },	// 21	PAvg1
	{ 15,  16,   0,   0,   0,   0  },	// 22	AAvg1
} };

const Matrix<int, 23, 3> Sensors::menuMap =
{ {
	// [0] maps ReadingType:
	//		Humidity:		0
	//		Temperature:	1
	//		Pressure:		2
	//		Altitue:		3
	// [1] maps FontType index for left display
	// [2] maps FontType index for right display

	// ReadingType	Left Font	Right Font
	{		0,			0,			0		},	// 0	None
	{		0,			1,			0		},	// 1	H1
	{		0,			1,			0		},	// 2	H2
	{		0,			1,			0		},	// 3	H3
	{		0,			1,			0		},	// 4	H4
	{		0,			1,			0		},	// 5	H5
	{		0,			1,			0		},	// 6	H6
	{		1,			1,			0		},	// 7	T1
	{		1,			1,			0		},	// 8	T2
	{		1,			1,			0		},	// 9	T3
	{		1,			1,			0		},	// 10	T4
	{		1,			1,			0		},	// 11	T5
	{		1,			1,			0		},	// 12	T6
	{		2,			2,			3		},	// 13	P1
	{		2,			2,			3		},	// 14	P2
	{		3,			1,			1		},	// 15	A1
	{		3,			1,			1		},	// 16	A2
	{		0,			0,			0		},	// 17	HAvg1
	{		1,			0,			0		},	// 18	TAvg1
	{		0,			0,			0		},	// 19	HAvg2
	{		1,			0,			0		},	// 20	TAvg2
	{		2,			0,			0		},	// 21	PAvg1
	{		3,			0,			0		},	// 22	AAvg1
} };
