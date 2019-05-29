#include "RotaryEncoder.h"

namespace HENRY
{
RotaryEncoder::RotaryEncoder()
	: m_pinA(0), m_pinB(0), m_position(0), m_flagA(0), m_flagB(0)
{
}

RotaryEncoder::RotaryEncoder(unsigned char pin_a, unsigned char pin_b)
	: m_pinA(pin_a), m_pinB(pin_b), m_position(0), m_flagA(0), m_flagB(0)
{
	pinMode(m_pinA, INPUT_PULLUP);
	pinMode(m_pinB, INPUT_PULLUP);
}

bool RotaryEncoder::init(unsigned char pin_a, unsigned char pin_b)
{
	if (pin_a == pin_b)
		return false;
	m_pinA = pin_a;
	m_pinB = pin_b;
	pinMode(m_pinA, INPUT_PULLUP);
	pinMode(m_pinB, INPUT_PULLUP);
	return true;
}

void RotaryEncoder::update()
{
	/* TODO: figure out how to read the R.E. */
}
} // namespace HENRY
