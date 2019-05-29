#pragma once

namespace HENRY {
	enum INIT {
		SUCCESS = 0,	
		MOT_FAIL = 1 << 0,
	};
	
	namespace Boat {
		bool init();
	}
}


// #pragma once
// #include "Magnetometer.h"
// #include "RotaryEncoder.h"
// #include "Motor.h"
// #include "GPS.h"

// namespace HENRY
// {
// // BoatProperties
// // This struct will be used for defining the various
// // settings in the boat such as the no sail zone angle
// // and specific ports on the arduino board.
// struct BoatProperties
// {
// 	// all necessary properties should be set in here.
// 	float noSailZoneAngle;
// };

// enum INIT
// {
// 	SUCCESS = 0,
// 	MAG_FAIL = 1 << 0,
// 	ROT_FAIL = 1 << 1,
// 	MOT_FAIL = 1 << 2,
// 	GPS_FAIL = 1 << 3,
// };

// // Boat
// class Boat
// {
// 	BoatProperties *m_prop;
// 	Magnetometer m_mag;
// 	RotaryEncoder m_rot;
// 	Motor m_mot1, m_mot2;
// 	GPS m_gps;

// 	bool m_isTrackingPoints;
// 	Math::dvec2 *m_trackPoints;
// 	unsigned short m_trackPointCount, m_currentTrackPoint;

// 	void sailTowards(Math::dvec2 &point);
// 	void setSails(float value) {}
// 	void setRudder(float value) {}

// public:
// 	Boat() {}
// 	~Boat() {}
	
// 	Boat(BoatProperties *prop);

// 	unsigned char init();
// 	unsigned char init(BoatProperties *prop);
// 	void update();

// 	inline Math::dvec2 &getPos() { return m_gps.getCoord(); }
// 	inline Math::dvec3 &getRot() { return m_mag.getRot(); }
// 	void setTargetPos(Math::dvec2 &point);
// 	void setTargetRoute(Math::dvec2 *points, unsigned short pointCount);
// };
// } // namespace HENRY
