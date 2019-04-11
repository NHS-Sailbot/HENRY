#pragma once
#include <Arduino.h>

namespace HENRY
{
// Motor
// pin a is power (through "PWM" pulse width modulation) - analog
// pin b is direction - digital, either HIGH or LOW
class Motor
{
	long m_prevPos, m_currPos;
	unsigned char m_pinA, m_pinB;

public:
	Motor();
	Motor(unsigned char pin_a, unsigned char pin_b);

	bool init(unsigned char pin_a, unsigned char pin_b);
	void update();

	inline unsigned short getPos() { return m_currPos; }
	inline void setPow(unsigned int pow) { analogWrite(m_pinA, pow); }
	inline void setDir(unsigned int dir) { digitalWrite(m_pinB, dir); }
};
} // namespace HENRY
