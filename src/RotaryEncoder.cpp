#include "RotaryEncoder.h"
#include <Arduino.h>

namespace HENRY {
	namespace RotaryEncoder {
		static byte s_pinA = 2, s_pinB = 3; // These have to be 2 and 3 for some reason.
		int encoderValue = 0, lastEncoded = 0;

		void internal_RotaryCallback()
		{
			int MSB = digitalRead(s_pinA); //MSB = most significant bit
			int LSB = digitalRead(s_pinB); //LSB = least significant bit
			int encoded = (MSB << 1) | LSB;
			int sum = (lastEncoded << 2) | encoded;
			if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
				encoderValue++;
			if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
				encoderValue--;
			lastEncoded = encoded;
		}
		uint init()
		{
			if (s_pinA == s_pinB)
				return false;
			pinMode(s_pinA, INPUT_PULLUP);
			pinMode(s_pinB, INPUT_PULLUP);

			//This part may not be needed//
			digitalWrite(s_pinA, HIGH); //turn pullup resistor on
			digitalWrite(s_pinB, HIGH); //turn pullup resistor on

			attachInterrupt(0, &internal_RotaryCallback, CHANGE);
			attachInterrupt(1, &internal_RotaryCallback, CHANGE);
			return true;
		}
		int getValue()
		{
			return encoderValue;
		}

	} // namespace RotaryEncoder
} // namespace HENRY
