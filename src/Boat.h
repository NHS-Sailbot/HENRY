#pragma once

#include "internal.h"
#include "Vector.h"
#include <Servo.h>

namespace HENRY {
	enum INIT {
		SUCCESS = 0,
		MOT_FAIL = 1 << 0,
		GPS_FAIL = 1 << 1,
		MAG_FAIL = 1 << 2,
		ROT_FAIL = 1 << 3,
		R_C_FAIL = 1 << 3,
	};

	namespace Boat {
		uint init();
		void update();
		void setTargetRoute(Math::dvec2 *points, ushort pointCount);
	} // namespace Boat
} // namespace HENRY
