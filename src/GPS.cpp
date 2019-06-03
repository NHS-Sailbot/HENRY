#include "GPS.h"
#include <TinyGPS++.h>

namespace HENRY {
	namespace GPS {
		static TinyGPSPlus s_tinyGPS;
		static Math::dvec2 s_coordinates;
		uint init()
		{
			Serial1.begin(9600);
			return true;
		}
		void update()
		{
			while (Serial1.available())
				s_tinyGPS.encode(Serial1.read());
			if (s_tinyGPS.location.isUpdated()) {
				s_coordinates.x = s_tinyGPS.location.lng();
				s_coordinates.y = s_tinyGPS.location.lat();
			}
		}
		const Math::dvec2 &getPos() { return s_coordinates; }
		double getLongitude() { return s_coordinates.x; }
		double getLatitude() { return s_coordinates.y; }
	} // namespace GPS
} // namespace HENRY
