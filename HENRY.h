#pragma once

#define NSZ_ANGLE            30

#define RUDDER_MAX         2100
#define RUDDER_MIN            0
#define WINCH_MAX          2100
#define WINCH_MIN             0

#define PIN_GPS_RX           11
#define PIN_GPS_TX           10
#define PIN_rESS             53
#define PIN_RUDDER           30
#define PIN_WINCH            31

namespace HENRY
{
	int Ascii_ISO_3D(const char * data, const unsigned short width);
}
