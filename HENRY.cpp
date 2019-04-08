#include "HENRY.h"
#include "ext-Encoder.h"

namespace HENRY
{
	
	Motor::Motor(unsigned short pin_a, unsigned short pin_b)
		: m_pinA(pin_a), m_pinB(pin_b)
	{
		init();
	}

	void Motor::init()
	{
		pinMode(m_pinA, OUTPUT);
		pinMode(m_pinB, OUTPUT);
	}

	void Motor::init(unsigned short pin_a, unsigned short pin_b)
	{
		m_pinA = pin_a;
		m_pinB = pin_b;
		pinMode(pin_a, OUTPUT);
		pinMode(pin_b, OUTPUT);
	}

	inline void Motor::update()
	{

	}
}
