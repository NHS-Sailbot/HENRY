#pragma once
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

namespace HENRY
{

	class TOFSensor
	{
		Adafruit_VL53L0X m_sensor;
		VL53l0X_RangingMeasurementData_t m_measure;
	public:
		TOFSensor();
		~TOFSensor() {}

		unsigned short getDist();
	};

}
