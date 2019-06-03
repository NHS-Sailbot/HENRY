#pragma once

#include "internal.h"
#include "Vector.h"

namespace HENRY {
	// Orientation Sensor
	namespace Magnetometer {
		uint init();
		Math::dvec3 getRot();
	} // namespace Magnetometer
}; // namespace HENRY
