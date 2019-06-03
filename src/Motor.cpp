#include "Motor.h"
#include "Vector.h"

namespace HENRY {
	namespace Motor {
		uint init(MotorData &m, byte pin_pwm, byte pin_dir)
		{
			if (pin_pwm == pin_dir)
				return false;
			m.pinPWM = pin_pwm, m.pinDir = pin_dir;
			pinMode(pin_pwm, OUTPUT);
			pinMode(pin_dir, OUTPUT);

			m.pinInA = 3, m.pinInB = 4;
			pinMode(m.pinInA, INPUT);
			pinMode(m.pinInB, INPUT);
			return true;
		}
		void update(MotorData &m)
		{
			if (digitalRead(m.pinInA) == HIGH) {
				if (digitalRead(m.pinInB) == LOW)
					m.currPos++;
				else
					m.currPos--;
			} else {
				if (digitalRead(m.pinInB) == LOW)
					m.currPos--;
				else
					m.currPos++;
			}
		}
	} // namespace Motor
} // namespace HENRY
