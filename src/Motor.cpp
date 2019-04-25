#include "Motor.h"

namespace HENRY
{
Motor::Motor()
	: m_pinA(0), m_pinB(0)
{
}

Motor::Motor(unsigned char pin_a, unsigned char pin_b)
	: m_pinA(pin_a), m_pinB(pin_b)
{
}

bool Motor::init(unsigned char pin_a, unsigned char pin_b)
{
	if (pin_a == pin_b)
		return false;
	m_pinA = pin_a;
	m_pinB = pin_b;
	pinMode(pin_a, OUTPUT);
	pinMode(pin_b, OUTPUT);
	return true;
}

void Motor::update()
{
	setPow(255);
	setDir(HIGH);
}
} // namespace HENRY
