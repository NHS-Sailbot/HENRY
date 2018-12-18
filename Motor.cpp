#include "Motor.h"



namespace HENRY
{

	/* Motor is a class that, with a pair of pins, controls a Linear
	Actuator with a motor controller using pulse width modulation.
	when used in code, just instantiate the object and call the
	function '.drive(a, b)' to control the motor. parameter 'a'
	controls how fast the motor runs and can be anything from 0 - 255.
	parameter b controls the direction and can either be HIGH or LOW.

		for example:
	------------------------------
		HENRY::Motor myMotor(3, 2); // set PWM pin to 3 and direction pin to 2
		or - - - - - myMotor(MOTOR_A)
		...
		myMotor.drive(255, HIGH); // set to push at max speed
		delay(1000) // push out for one second

		myMotor.drive(255, LOW); // set to pull at max speed
		delay(1000) // pull in for one second

	*/

	/* The main constructor just takes in and assigns specific 
	pins on the board for the */
	Motor::Motor(unsigned char pin_pwm, unsigned char pin_dir)
		: m_pinPWM(pin_pwm), m_pinDir(pin_dir) 
	{
	}

	/* This secendary constructor takes in a boolean that's meant to 
	be used with the macros "MOTOR_A" and "MOTOR_B" to assign the pins
	since the motor controller hard codes the pins usable. */
	Motor::Motor(bool motor_number)
		: m_pinPWM(motor_number ? 3 : 9), m_pinDir(motor_number ? 2 : 8) {}

	/* The analog pulse width modulation pin (PWM for short) controls
	how fast or slow the motor travels, whereas the digital direction
	pin sends either HIGH or LOW dictating whether to go in or out. */
	void Motor::init()
	{
		pinMode(m_pinPWM, 1);
		pinMode(m_pinDir, 1);
	}

	/* This function just sets the values for which the motor runs with.
	By this I mean once this is called, the motor will run at the
	specified speed and direction until it is told to do otherwise, thus
	if you call this function once and only once with values telling it
	to go out as fast as possible, it will go out ... as fast as
	possible, forever.*/
	void Motor::drive(unsigned char power, unsigned char direction)
	{
		analogWrite(m_pinPWM, power);
		digitalWrite(m_pinDir, direction);
	}

	/* This setLength(...) function calculates the distance that the 
	actuator is out and extends/retracts it based on how far the 
	specified length is in relation to it. */
	void Motor::setLength(unsigned short mm)
	{
		unsigned short currentLen = m_tofs.getDist();
		/* this next two lines are just a logical simplification of a pair
		of if/else statements. I do this because it negates the creation
		and destruction of a single stack frame on runtime.
		-------------------------------------------------
		if (abs(currentLen - mm) < MOTOR_MIN_RNG))
			analogWrite(m_pinPWM, 0);
		else
			analogWrite(m_pinPWM, 255); */
		analogWrite(m_pinPWM, (abs(currentLen - mm) < MOTOR_MIN_RNG) * 255);
		digitalWrite(m_pinDir, currentLen < mm);
	}

}
