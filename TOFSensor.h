#pragma once
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <vl53l0x_def.h>

namespace HENRY
{

	class TOFSensor
	{
		Adafruit_VL53L0X m_sensor;
		VL53L0X_RangingMeasurementData_t m_measure;
	public:
		TOFSensor() {}
		~TOFSensor() {}

		void init();
		unsigned short getDist();
	};

}
