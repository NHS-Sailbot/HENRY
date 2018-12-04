#pragma once
#include <Arduino.h>

class Receiver
{
	unsigned short m_pin;
public:
	Receiver(unsigned short pin);
	~Receiver();
	float read();
};



