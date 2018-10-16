#include "HENRY.h"

#include "Arduino.h"

namespace HENRY
{
	/*------------------------------------------
	|                                          |
	|                 FUNCTIONS                |
	|                                          |
	------------------------------------------*/
	
	/* A function that takes in a set of data
	that represents an image and it's width,
	and it prints it to the serial monitor
	in isometric 3D.

		for example:
	------------------------------
		const char * image =
		" # # # #"
		"# # # # "
		"  ##  ##"
		"##  ##  ";

		Ascii_ISO_3D(image, 8);
	------------------------------*/
	bool Ascii_ISO_3D(const char * data, const unsigned short width)
	{
		/*---------------------
		|   bullet proofing   |
		---------------------*/
		/* run through the char set until it 
		finds an end character. */
		unsigned int dataLen = 0;
		while (data[dataLen] != '\0')
			dataLen++;
		/* if the number of characters in the 
		input data is indivisible by the input
		width, return a failure code, ending the
		execution of the function. */
		if (dataLen % width != 0)
		{
			Serial.println("ERROR::ASCII_ISO_3D: Data does not match width!");
			Serial.println("  Possible causes: input width is not the width of the image");
			Serial.println("                   char set does not have a constant width");
			return 1;
		}
		/*---------------------
		|   drawing process   |
		---------------------*/
		/* calculate the height of the "image" by 
		dividing the number of characters in the 
		image by the input width. */
		const unsigned short height = dataLen / width;
		/* naive way of having an adjustable value. */
		const unsigned short scale = 1;
		/* run through all of the input characters. */
		for (unsigned short y = 0; y < height * scale; y++)
		{
			for (unsigned short x = 0; x < width * scale; x++)
			{
				/* test all the values to change the representation 
				of the image to be isometric 3D. */
				unsigned short currentIndex = x / scale + y / scale * width;
				if (data[currentIndex] == '#')
					Serial.print("\\\\");
				else
				{
					if (data[currentIndex + 1] == ' ')
					{
						if (data[currentIndex - width] == '#')
						{
							if (data[currentIndex - width + 1] == ' ')
								Serial.print("/ ");
							else
								Serial.print("//");
						}
						else if (data[currentIndex - width + 1] == '#')
							Serial.print("\\/");
						else
							Serial.print("  ");
					}
					else if (data[currentIndex - width + 1] == '#')
					{
						if (data[currentIndex - width] == '#')
							Serial.print("//");
						else
							Serial.print("\\/");
					}
					else
					{
						if (data[currentIndex - width] == '#')
							Serial.print("//");
						else
							Serial.print(" /");
					}
				}
			}
			/* Start a new line after all the characters 
			have been printed in the line. */
			Serial.println(' ');
			/* offset all the lines at the beginning 
			to make it look slanted. */
			for (unsigned short i = 0; i < y + 1; i++)
				Serial.print(' ');
		}
		return 0;
	}
	
	
	
	
	
	/*------------------------------------------
	|                                          |
	|                  OBJECTS                 |
	|                                          |
	------------------------------------------*/
	
	/* 'Boat' is a class that holds all the information
	and methods for the boats to be autonomous.	Its 
	constructor takes a pointer to a struct that holds 
	all the necessary pins and values for the boat to 
	run.

		for example:
	------------------------------
		BoatProperties myProp = {
			30,     // NSZ_ANGLE 
			2100,   // RUDDER_MAX
			0,      // RUDDER_MIN
			2100,   // WINCH_MAX 
			0,      // WINCH_MIN 
			11,     // PIN_GPS_RX
			10,     // PIN_GPS_TX
			53,     // PIN_rESS  
			30,     // PIN_RUDDER
			31,     // PIN_WINCH 
		}

		Boat myBoat(&myProp);
	------------------------------*/
	Boat::Boat(BoatProperties * properties) 
		: m_prop(properties), currentGPScoordinate(0), m_searchPattern(nullptr)
	{
		this->systemValidate();
	}
	
	Boat::~Boat() {}
	
	bool Boat::systemValidate()
	{
		//printing
	}
	
	/* Method that fetches the value from the boat's 
	rotary encoder. */
	unsigned int Boat::rotaryEncoderValue()
	{
		/* Initialise the SPI bus. */
		SPI.begin();
		/* Set the SPI settings. */
  		SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
		/* Write to the rotary encoder's pin a value of 0. */
		digitalWrite(m_prop.m_pin_rESS, LOW);
		unsigned int value = SPI.transfer(0xFF);
		/* Bit shift left the bits of 'value'. */
		value << 8;
		/* Bitwise OR statement between 'value' and the return
		value of SPI.transfer.(0xFF). the OR statement turns
		all bits that are on in either of them on in the result.*/
		value | SPI.transfer.(0xFF);
		/* Bit shift right the bits of 'value'*/
		value >> 5;
		/* Write to the rotary encoder's pin a value of 1. */
		digitalWrite(m_prop.m_pin_rESS, HIGH);
		/* Free the SPI bus. */
		SPI.end();
		return value;
	}
	
	/* Method that converts the boat's rotary encoder's 
	value to an angle that is in radians. */
	float Boat::rotaryEncoderInRadians()
	{
		/* rotaryEncoderValue() returns an unsigned integer with
		value from 0 to 1023, so we divide it by its 'maximum value'
		of 1024 (its basically like saying 1023 is 99% around the 
		circle and then 1024 is just back to the start, so it outputs
		0). we then multiply it by TAU or 2*PI to map it from 0 to 6.28.*/
		return (float)this->rotaryEncoderValue() / 1024 * 6.2831853071;
	}

	/* Method that returns the distance between two 
	GPS coordinates whilst assuming the Earth is a 
	cartesian plane on which x is longitude and 
	y is latitude. */
	double Boat::GPScoordinateDistance()
	{
		/* Using the pythagorean theorem to get the distance between two points
		on a cartesian plane since arc length calculations are too computationally
		intesive for what difference they make on the scale of 500 feet.*/
		return sqrt(GPScoordinateDiffLong() * GPScoordinateDiffLong() + GPScoordinateDiffLat() * GPScoordinateDiffLat()) * 364610.4;
	}
	
	/* Method that returns the difference between the 
	currently assigned search pattern longitude and the
	GPS's current longitude. */
	double Boat::GPScoordinateDiffLong()
	{
		if (m_searchPatternLongitude && currentGPScoordinateIndex < m_numSearchPatternCoordinates)
			return m_searchPatternLongitude[currentGPScoordinateIndex] - gps.location.lng();
	}
	
	/* Method that returns the difference between the 
	currently assigned search pattern latitude and the 
	GPS's current latitude. */
	double Boat::GPScoordinateDiffLat()
	{
		if (m_searchPatternLatitude && currentGPScoordinateIndex < m_numSearchPatternCoordinates)
			return m_searchPatternLatitude[currentGPScoordinateIndex] - gps.location.lat();
	}
}
