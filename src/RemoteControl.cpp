#include "RemoteControl.h"
#include <Arduino.h>

namespace HENRY {
	namespace RemoteControl {
		int init()
		{
			pinMode(2, INPUT); // ask Camden about pins
			pinMode(3, INPUT); // analog pwm pins
			pinMode(4, INPUT); // analog pwm pins
			pinMode(5, INPUT); // analog pwm pins
			pinMode(6, INPUT); // analog pwm pins
			return 1;
		}
		int getLeftHoriz() { return pulseIn(2, HIGH, 25000); } // figure out what pins to use
		int getLeftVert() { return pulseIn(3, HIGH, 25000); } // figure out what pins to use
		int getRightHoriz() { return pulseIn(4, HIGH, 25000); } // figure out what pins to use
		int getRightVert() { return pulseIn(5, HIGH, 25000); } // figure out what pins to use
		int getControlMode() { return pulseIn(6, HIGH, 25000); } // figure out what pins to use
	} // namespace RemoteControl
} // namespace HENRY