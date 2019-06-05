#include "RotaryEncoder.h"
#include <Arduino.h>

namespace HENRY {
	namespace RotaryEncoder {
		constexpr static const byte pinData = 53, pinClock = 51, pinCS = 49;
		constexpr static const float encoderToRadiansRatio = 0.00613592315;
		static int encoderValue = 0;

		uint init()
		{
			pinMode(pinCS, OUTPUT);
			pinMode(pinClock, OUTPUT);
			pinMode(pinData, INPUT);

			digitalWrite(pinClock, HIGH);
			digitalWrite(pinCS, LOW);
			return true;
		}
		float getValue()
		{
			digitalWrite(pinCS, HIGH);
			digitalWrite(pinCS, LOW);
			encoderValue = 0;
			for (byte i = 0; i < 10; ++i) {
				digitalWrite(pinClock, LOW);
				digitalWrite(pinClock, HIGH);
				byte b = digitalRead(pinData) == HIGH ? 1 : 0;
				encoderValue += b * pow(2, 10 - (i + 1));
			}
			for (byte i = 0; i < 6; ++i) {
				digitalWrite(pinClock, LOW);
				digitalWrite(pinClock, HIGH);
			}
			digitalWrite(pinClock, LOW);
			digitalWrite(pinClock, HIGH);
			return float(encoderValue); //* encoderToRadiansRatio;
		}
	} // namespace RotaryEncoder
} // namespace HENRY
