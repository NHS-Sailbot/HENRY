#pragma once

#include <Arduino.h>
#include "internal.h"

namespace HENRY {
	struct MotorData {
		byte pinPWM, pinDir, pinInA, pinInB;
		int shouldPull = 1;
		long currPos;
	};
	namespace Motor {
		uint init(MotorData &m, byte pin_pwm, byte pin_dir);
		void reset(MotorData &m);
		void update(MotorData &m);
		void setAngle(MotorData &m, float angle);
		inline void setPow(const MotorData &m, uint pow) { analogWrite(m.pinPWM, pow); }
		inline void setDir(const MotorData &m, byte dir) { digitalWrite(m.pinDir, dir); }
	} // namespace Motor
} // namespace HENRY
