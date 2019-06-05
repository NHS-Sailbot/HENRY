#include "RemoteControl.h"
#include <Arduino.h>

namespace HENRY {
	namespace RemoteControl {
		int init()
		{
			pinMode(9, INPUT);
			pinMode(10, INPUT);
			pinMode(11, INPUT);
			return 1;
		}
		static float lastLeftReading = 0.5;
		float getLeftStick()
		{
			float result = float(pulseIn(9, HIGH, 25000));
			if (result > 1 || result < 0)
				return result;
			else {
				lastLeftReading = result;
				return result;
			}
		}
		static float lastRightReading = 0.5;
		float getRightStick()
		{
			float result = float(pulseIn(10, HIGH, 25000));
			if (result > 1 || result < 0)
				return result;
			else {
				lastRightReading = result;
				return result;
			}
		}
		int getControlMode()
		{
			return pulseIn(10, HIGH, 25000);
			return RemoteControl::Mode::Remote;
		} // figure out what pins to use
	} // namespace RemoteControl
} // namespace HENRY
