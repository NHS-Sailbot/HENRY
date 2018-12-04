#pragma once
#include <Arduino.h>

namespace HENRY
{
	class RotaryEncoder
	{
	private:
		unsigned char m_pinA, m_pinB;
		byte m_position, m_flagA, m_flagB;
	public:
		RotaryEncoder(unsigned char pin_a, unsigned char pin_b);
		~RotaryEncoder() {}

		void RotaryEncoder::update();
	};
}
