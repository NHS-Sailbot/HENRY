#pragma once
#include <Arduino.h>

namespace HENRY
{

	class Receiver
	{
		unsigned char m_pin;
	public:
		Receiver(unsigned char pin);
		~Receiver() {}

		float read();
	};

}
