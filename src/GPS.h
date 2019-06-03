#pragma once

#include "internal.h"
#include "Vector.h"

namespace HENRY {
	namespace GPS {
		uint init();
		void update();
		const Math::dvec2 &getPos();
		double getLongitude();
		double getLatitude();
	} // namespace GPS
} // namespace HENRY
