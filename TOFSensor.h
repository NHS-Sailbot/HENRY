#pragma once
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

namespace HENRY
{

	class TOFSensor
	{
		Adafruit_VL53L0X m_sensor;
	public:
		TOFSensor();
		~TOFSensor() {}

		unsigned short getRange();
	};

}