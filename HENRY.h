#pragma once

#include <SPI.h>
#include <Servo.h>
#include <TinyGPS++.h>
#include <Adafruit_BNO055.h>

#define TAU 6.2831853071

namespace HENRY
{
	bool Ascii_ISO_3D(const char * data, const unsigned short width);
	
	
}
