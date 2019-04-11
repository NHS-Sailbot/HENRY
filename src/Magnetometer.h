#pragma once
#include <Adafruit_BNO055.h>
#include "Vector.h"

namespace HENRY
{
// Orientation Sensor
class Magnetometer
{
	Adafruit_BNO055 m_mag;
	Math::dvec3 m_rot;

public:
	Magnetometer();
	~Magnetometer() {}

	bool init();
	void update();
	inline Math::dvec3 &getRot() { return m_rot; }
};
} // namespace HENRY
