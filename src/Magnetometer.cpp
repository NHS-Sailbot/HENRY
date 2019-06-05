#include "Magnetometer.h"
#include <Adafruit_BNO055.h>
#include <utility/quaternion.h>

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
			imu::Quaternion quat = s_mag.getQuat();
			auto v = quat.toEuler();
			return {v.x(), v.y(), v.z()};
		}
	} // namespace Magnetometer
} // namespace HENRY
