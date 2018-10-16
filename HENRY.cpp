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
	
	/* A Class that holds all the information
	and methods for the boats to be autonomous.
	its constructor takes a pointer to a struct
	that holds all the necessary pins and values 
	for the boat to run.

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
	
	unsigned int Boat::rotaryEncoderValue()
	{
		SPI.begin();
  		SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
		digitalWrite(m_prop.m_pin_rESS, LOW);
		unsigned int value = SPI.transfer(0xFF);
		value << 8;
		value | SPI.transfer.(0xFF);
		value >> 5;
		digitalWrite(m_prop.m_pin_rESS, HIGH);
		SPI.end();
		return value;
	}
	
	float Boat::rotaryEncoderInRadians()
	{
		return (float)this->rotaryEncoderValue() / 1024 * 3.14159265 * 2;
	}

	/* Method that returns the distance between two GPS coordinates 
	whilst assuming the Earth is a cartesian plane on which
	x is longitude and y is latitude. */
	double Boat::GPScoordinateDistance()
	{
		return sqrt(GPScoordinateDiffLong() * GPScoordinateDiffLong() + GPScoordinateDiffLat() * GPScoordinateDiffLat()) * 364610.4;
	}
	
	/* Method that returns the difference between the currently assigned
	search pattern longitude and the GPS's current longitude. */
	double Boat::GPScoordinateDiffLong()
	{
		if (m_searchPatternLongitude && currentGPScoordinateIndex < m_numSearchPatternCoordinates)
			return m_searchPatternLongitude[currentGPScoordinateIndex] - gps.location.lng();
	}
	
	/* Method that returns the difference between the currently assigned
	search pattern latitude and the GPS's current latitude. */
	double Boat::GPScoordinateDiffLat()
	{
		if (m_searchPatternLatitude && currentGPScoordinateIndex < m_numSearchPatternCoordinates)
			return m_searchPatternLatitude[currentGPScoordinateIndex] - gps.location.lat();
	}
}