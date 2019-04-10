#include "RotaryEncoder.h"

namespace HENRY
{
/* TODO: Add descriptions and usage documentation. */
RotaryEncoder::RotaryEncoder(unsigned char pin_a, unsigned char pin_b)
	: m_pinA(pin_a), m_pinB(pin_b), m_position(0), m_flagA(0), m_flagB(0)
{
	pinMode(m_pinA, INPUT_PULLUP);
	pinMode(m_pinB, INPUT_PULLUP);
}

void RotaryEncoder::update()
{
	/* TODO: figure out how to read the R.E. */
}
} // namespace HENRY
