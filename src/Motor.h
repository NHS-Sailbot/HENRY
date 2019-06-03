#pragma once

#include <Arduino.h>
#include "internal.h"

namespace HENRY {
	struct MotorData {
		byte pinPWM, pinDir, pinInA, pinInB;
		long currPos;
	};
	namespace Motor {
		uint init(MotorData &m, byte pin_pwm, byte pin_dir);
		void update(MotorData &m);
		inline void setPow(const MotorData &m, uint pow) { analogWrite(m.pinPWM, pow); }
		inline void setDir(const MotorData &m, byte dir) { digitalWrite(m.pinDir, dir); }
	} // namespace Motor
} // namespace HENRY
