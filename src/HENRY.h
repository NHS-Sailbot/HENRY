#pragma once
#include <Arduino.h>

#include "GPS.h"
#include "Magnetometer.h"
#include "Motor.h"
#include "Boat.h"
#include "RotaryEncoder.h"
#include "RemoteControl.h"

namespace HENRY {
	bool Ascii_ISO_3D(const char *message);
	bool Ascii_ISO_3D(const char *data, const unsigned short width);
	void printInfo();
} // namespace HENRY
