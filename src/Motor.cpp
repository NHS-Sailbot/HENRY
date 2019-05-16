#include "Motor.h"

namespace HENRY
{
static void motorInterrupt()
{
}

Motor::Motor()
	: m_pinPWM(0), m_pinDIR(0), m_pinInA(0), m_pinInB(0)
{
}

Motor::Motor(unsigned char pin_pwm, unsigned char pin_dir)
{
	init(pin_pwm, pin_dir);
}

bool Motor::init(unsigned char pin_pwm, unsigned char pin_dir)
{
	if (pin_pwm == pin_dir)
		return false;
	m_pinPWM = pin_pwm, m_pinDIR = pin_dir;
	pinMode(pin_pwm, OUTPUT);
	pinMode(pin_dir, OUTPUT);

	m_pinInA = 3, m_pinInB = 4;
	pinMode(m_pinInA, INPUT);
	pinMode(m_pinInB, INPUT);
	return true;
}

void Motor::update()
{
	if (digitalRead(m_pinInA) == HIGH)
	{
		if (digitalRead(m_pinInB) == LOW)
			m_currPos++;
		else
			m_currPos--;
	}
	else
	{
		if (digitalRead(m_pinInB) == LOW)
			m_currPos--;
		else
			m_currPos++;
	}
}
} // namespace HENRY
