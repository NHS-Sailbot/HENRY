#pragma once
#include <Arduino.h>

#define HENRY_CREATE_MOTOR_INTERRUPT(motName, funcName) \
	void funcName() { motName.update(); }
#define HENRY_ATTACH_MOTOR_INTERRUPT(num, funcName) \
	attachInterrupt(num, funcName, CHANGE)

namespace HENRY
{
class Motor
{
	long m_currPos;
	unsigned char m_pinPWM, m_pinDIR, m_pinInA, m_pinInB;

public:
	Motor();
	Motor(unsigned char pin_a, unsigned char pin_b);

	bool init(unsigned char pin_a, unsigned char pin_b);
	void update();

	inline void setPow(unsigned int pow) { analogWrite(m_pinPWM, pow); }
	inline void setDir(unsigned int dir) { digitalWrite(m_pinDIR, dir); }
	inline long getPos() { return m_currPos; }
};
} // namespace HENRY
