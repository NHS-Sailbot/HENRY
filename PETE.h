#pragma once

namespace PETE
{
	class Motor
	{
		unsigned char m_pinPWM,
			m_pinDirection;
	public:
		Motor(unsigned char pin_pwm, unsigned char pin_dir);
		~Motor() {}

		void drive(unsigned char power, unsigned char direction);
	};
}
