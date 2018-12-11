#pragma once
#include <Arduino.h>
#include <TinyGPS++.h>
#include "vec.h"

namespace HENRY
{

	class GPS
	{
		TinyGPSPlus m_gps;
		Math::vec2 m_loc;
	public:
		GPS() {};
		~GPS() {}

		void begin();
		void update();
		double getLongitude() { return m_loc.y; }
		double getLatitude() { return m_loc.x; }
	};

}
