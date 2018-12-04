#include "Receiver.h"


Receiver::Receiver(unsigned short pin)
	: m_pin(pin)
{
	pinMode(m_pin, INPUT);
}


Receiver::~Receiver()
{
}


float Receiver::read()
{
	int reading = pulseIn(m_pin, HIGH, 25000);
	float value = float((reading - 1490) / 40) * 9 + 90;
	return value;
}

