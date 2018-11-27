#include "PETE.h"

namespace PETE
{

	/* Motor is a class that, with a pair of pins,
	controls a Linear Actuator with a motor controller
	using pulse width modulation. when used in code, 
	just instantiate the object and call the function
	'.drive(a, b)' to control the motor. parameter 'a'
	controls how fast the motor runs and can be anything
	from 0 - 255. parameter b controls the direction and 
	can either be HIGH or LOW.
		
		for example:
	------------------------------
		PETE::Motor myPeteMotor(3, 2); // set PWM pin to 3 and direction pin to 2
		...
		myPeteMotor.drive(255, HIGH); // set to push at max speed
		delay(1000) // push out for one second

		myPeteMotor.drive(255, LOW); // set to pull at max speed
		delay(1000) // pull in for one second

	*/

	/* The analog pulse width modulation pin (PWM for short) controls
	how fast or slow the motor travels, whereas the digital direction
	pin sends either HIGH or LOW dictating whether to go in or out. */
	Motor::Motor(unsigned char pin_pwm, unsigned char pin_dir)
		m_pinPWM()
	{
		pinMode(pin_pwm, OUTPUT);
		pinMode(pin_dir, OUTPUT);
	}

	/* This function just sets the values for which the motor runs with.
	By this I mean once this is called, the motor will run at the specified
	speed and direction until it is told to do otherwise, thus if you call
	this function once and only once with values telling it to go out as 
	fast as possible, it will go *out* ... as fast as possible forever.*/
	void Motor::drive(unsigned char power, unsigned char direction)
	{
		digitalWrite(m_pinDirection, direction);
		analogWrite(m_pinPWM, power);
	}

}