#pragma once
#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "Math/vec.h"

namespace HENRY
{

	class GPS
	{
		unsigned char m_pinRX, m_pinTX;
		TinyGPSPlus m_gps;
		SoftwareSerial m_ss;
		Math::vec2 m_loc;
	private:
		GPS(unsigned char pin_rx, unsigned char pin_tx);
		~GPS() {}

		void update();
		double getLongitude() { return m_loc.y; }
		double getLatitude() { return m_loc.x; }
	};

}
