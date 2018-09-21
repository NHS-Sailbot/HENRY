#pragma once

#define NO_SAIL_ZONE_ANGLE 30

#define RUDDER_MAX  2100
#define RUDDER_MIN     0
#define WINCH_MAX   2100
#define WINCH_MIN      0

#define PIN_GPS_RX  11
#define PIN_GPS_TX  10
#define PIN_rESS    53
#define PIN_RUDDER  30
#define PIN_WINCH   31

namespace HENRY
{
	/* A function that takes in a set of data
	that represents an image and it's width, 
	and it prints it to the serial monitor 
	in isometric 3D.

		for example:
	------------------------------
		const char * image =
		" # # # #"
		"# # # # "
		"  ##  ##"
		"##  ##  ";

		Ascii_ISO_3D(image, 8);
	------------------------------
	*/
	int Ascii_ISO_3D(const char * data, const unsigned short width);
}