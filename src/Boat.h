#pragma once
#include "GPS.h"
#include "Magnetometer.h"

namespace HENRY
{
// BoatProperties
// This struct will be used for defining the various
// settings in the boat such as the no sail zone angle
// and specific ports on the arduino board.
struct BoatProperties
{
	// all necessary properties should be set in here.
};

enum INIT
{
	NONE = 0,
	BNO_FAIL = 1 << 0,
	GPS_FAIL = 1 << 1,
	ROT_FAIL = 1 << 2,
	SUCCESS = 1 << 3
};

// Boat
class Boat
{
	BoatProperties *m_prop;
	GPS m_gps;
	Magnetometer m_mag;

	bool m_isTrackingPoints;
	Math::dvec2 *m_trackPoints;
	unsigned short m_trackPointCount, m_currentTrackPoint;

	void sailTowards(Math::dvec2 &point);
	void setSails(float value) {}
	void setRudder(float value) {}

public:
	Boat(BoatProperties *prop);
	~Boat() {}

	unsigned int init();
	void update();

	inline Math::dvec2 &getPos() { return m_gps.getCoord(); }
	inline Math::dvec3 &getRot() { return m_mag.getRot(); }
	void setTargetPos(Math::dvec2 &point);
	void setTargetRoute(Math::dvec2 *points, unsigned short pointCount);
};
} // namespace HENRY
