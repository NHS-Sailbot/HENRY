#pragma once
#include <Arduino.h>
#include <Adafruit_BNO055.h>

namespace HENRY
{

	class Magnetometer
	{
		Adafruit_BNO055 

	public:
		Magnetometer();
		~Magnetometer();
	};

}