#pragma once
#include "GPS.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

namespace HENRY
{
	struct BoatProperties
	{
		// determine necessary properties
		// such as pin numbers etc.
	};

	enum EXIT_CODE
	{
		NONE = 0,
		BNO_FAIL = 1 << 0,
		GPS_FAIL = 1 << 1,
		ROT_FAIL = 1 << 2
	};

	class Boat
	{
		BoatProperties *m_prop;
		GPS m_gps;
		Adafruit_BNO055 m_mag;
		sensors_event_t m_bnoEvent;

		bool m_isTrackingPoints;
		Math::vec2 *m_trackPoints;
		unsigned short m_trackPointCount, m_currentTrackPoint;

		void sailTowards(const Math::vec2 &point);
		void setSails(float value);
		void setRudder(float value);

		// void turn(float angle);  TBD if to be implimented
		// void stabilize();

	public:
		Boat(BoatProperties *prop);
		~Boat() {}
		
		unsigned int init();
		void update();

		Math::vec2& getPos();
		Math::vec3& getRot();
		void setTargetPos(const Math::vec2 &point);
		void setTargetRoute(const Math::vec2 *points, unsigned short pointCount);
	};
}
