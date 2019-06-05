#include "RotaryEncoder.h"
#include <Arduino.h>

namespace HENRY {
	namespace RotaryEncoder {
		// pin A -> digital in
		// pin B -> clock
		// pin C -> digital out
		// pin D -> chip select

		// rotary encoder digital input: 26; clock: 24; cable select: 28 pin 3->GND 5->VCC

		constexpr static const byte pinData = 6, pinClock = 5, pinCS = 7;
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
			for (int i = 0; i < 10; i++) {
				digitalWrite(pinClock, LOW);
				digitalWrite(pinClock, HIGH);
				byte b = digitalRead(pinData) == HIGH ? 1 : 0;
				encoderValue += b * pow(2, 10 - (i + 1));
			}
			digitalWrite(pinClock, LOW);
			digitalWrite(pinClock, HIGH);
			return float(encoderValue) * encoderToRadiansRatio;
		}
	} // namespace RotaryEncoder
} // namespace HENRY
