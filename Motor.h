#pragma once
#include <Arduino.h>
#include "TOFSensor.h"

#define MOTOR_A 1
#define MOTOR_B 0
#define MOTOR_MIN_RNG 10

namespace HENRY
{

	class Motor
	{
	private:
		unsigned char m_pinPWM,
			m_pinDirection;
		TOFSensor m_tofs;
	public:
		Motor(unsigned char pin_pwm, unsigned char pin_dir);
		Motor(bool motor_number);
		~Motor() {}

		void drive(unsigned char power, unsigned char direction);
		void setLength(unsigned short mm);
	};

}
