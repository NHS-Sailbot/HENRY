#pragma once

namespace HENRY
{
	class Motor
	{
	private:
		unsigned char m_pinPWM,
			m_pinDirection;
	public:
		Motor(unsigned char pin_pwm, unsigned char pin_dir);
		Motor(bool motor_number);
		~Motor() {}

		void drive(unsigned char power, unsigned char direction);
	};
}

#define MOTOR_A 1
#define MOTOR_B 0
