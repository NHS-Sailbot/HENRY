#include "TOFSensor.h"



namespace HENRY
{

	/* TODO: Add descriptions and usage documentation. */
	void TOFSensor::init()
	{
		if (!m_sensor.begin())
			while (1);
	}


	unsigned short TOFSensor::getDist()
	{
		m_sensor.rangingTest(&m_measure, false);
		if (m_measure.RangeStatus == 4)
			return 0;
		return m_measure.RangeMilliMeter;
	}

}
