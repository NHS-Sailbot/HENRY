#pragma once
#include <Arduino.h>

// ems22a

namespace HENRY
{
class RotaryEncoder
{
private:
	unsigned char m_pinA, m_pinB;
	byte m_position, m_flagA, m_flagB;

public:
	RotaryEncoder();
	RotaryEncoder(unsigned char pin_a, unsigned char pin_b);
	~RotaryEncoder() {}

	bool init(unsigned char pin_a, unsigned char pin_b);
	void update();
};
} // namespace HENRY
