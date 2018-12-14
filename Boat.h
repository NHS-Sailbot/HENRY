#pragma once
#include "GPS.h"
#include "Motor.h"

namespace HENRY
{
	struct BoatProperties
	{
		double m_NoSailZoneAngle;
		unsigned char m_pin_rESS,
			m_pin_gyro,
			m_pin_rudder,
			m_pin_winch;
	};

	class Boat
	{
		BoatProperties * m_prop;
		GPS m_gps;
		Motor m_motorA, m_motorB;
	public:
		Boat(BoatProperties * prop);
		~Boat() {}
		
		void init();
	};
}