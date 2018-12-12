#include "TOFSensor.h"



namespace HENRY
{

	TOFSensor::TOFSensor()
	{
		if (!m_sensor.begin())
			while (1);

	}


	unsigned short TOFSensor::getRange()
	{
		VL53L0X_RangingMeasurementData_t measure;
		m_sensor.rangingTest(&measure, false);
		if (measure.RangeStatus == 4)
			return 0;
		return measure.RangeMillimeter;
	}

}
