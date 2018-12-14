#include "Boat.h"



namespace HENRY
{

	Boat::Boat(BoatProperties * prop)
		: m_prop(prop), m_motorA(MOTOR_A), m_motorB(MOTOR_B),

	{}
	

	void Boat::init()
	{
		/* This method executes all the initialization code needed 
		for all the needed components in the boat object to function. 
		This can be anything from starting up serial ports to setting
		the pinmode of their needed ports. */

		/* For further documentation, check the source files of the
		item in interest, they have all their methods and constructors
		commented. */

		/* Begins Serial1 which has been dedicated to the gps. */
		m_gps.init();
		/* Sets the motors pin modes. */
		m_motorA.init(); m_motorB.init();


	}

}