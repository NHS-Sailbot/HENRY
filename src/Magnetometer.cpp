#include "Magnetometer.h"
#include <Adafruit_BNO055.h>

namespace HENRY {
	namespace Magnetometer {
		static Adafruit_BNO055 s_mag;
		uint init()
		{
			if (!s_mag.begin())
				return false;
			delay(100);
			s_mag.setExtCrystalUse(false);
			return true;
		}
		Math::dvec3 getRot()
		{
			sensors_event_t event;
			s_mag.getEvent(&event);
			return {event.orientation.x,
				event.orientation.y,
				event.orientation.z};
		}
	} // namespace Magnetometer
} // namespace HENRY
