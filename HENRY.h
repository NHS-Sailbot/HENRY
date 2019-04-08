#pragma once
#include <Arduino.h>

namespace HENRY
{

	// pin a is power (through "PWM" pulse width modulation) - analog
	// pin b is direction - digital, either HIGH or LOW

	class Motor
	{
		long m_prevPos, m_currPos;
		unsigned short m_pinA, m_pinB;
	public:
		Motor() {}
		Motor(unsigned short pin_a, unsigned short pin_b);

		void init();
		void init(unsigned short pin_a, unsigned short pin_b);

		inline void update();

		inline unsigned short getPos() { return m_currPos; }
		
		inline void setPow(unsigned short pow)
			{ analogWrite(m_pinA, pow); }
		inline void setDir(unsigned short dir)
			{ digitalWrite(m_pinB, dir); }
	};

}
