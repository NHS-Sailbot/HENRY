#include "Motor.h"
#include "Vector.h"

namespace HENRY {
	namespace Motor {
		constexpr static const float winchAllTheWayOutRatio = 100000;

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
		void reset(MotorData &m)
		{
			m.currPos = 0;
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
			if (m.currPos < 0)
				m.shouldPull = 0;
			else
				m.shouldPull = 1;
		}
		void setAngle(MotorData &m, float angle)
		{
			if (m.shouldPull) {
				if ((float)m.currPos / winchAllTheWayOutRatio < angle) {
					setPow(m, 255);
					setDir(m, HIGH);
				} else {
					setPow(m, 255);
					setDir(m, LOW);
				}
			}
		}
	} // namespace Motor
} // namespace HENRY
