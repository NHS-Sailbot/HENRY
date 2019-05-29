#pragma once
#include <TinyGPS++.h>
#include "Vector.h"

namespace HENRY
{
// GPS is a wrapper class for the TinyGPS++ libraries
// proprietary gps object.
class GPS
{
	TinyGPSPlus m_gps;
	Math::dvec2 m_loc;

public:
	GPS() {}
	~GPS() {}

	bool init();
	void update();
	inline Math::dvec2 &getCoord() { return m_loc; }
	inline double getLongitude() { return m_loc.y; }
	inline double getLatitude() { return m_loc.x; }
};
} // namespace HENRY
