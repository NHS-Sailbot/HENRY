#include "Boat.h"

namespace HENRY
{

	/* 'Boat' is a class that holds all the information
	and methods for the boats to be autonomous.	Its
	constructor takes a pointer to a struct that holds
	all the necessary pins and values for the boat to
	run.

		for example:
	------------------------------
		BoatProperties myProp = {
			1.5,     // NSZ_ANGLE (radians)
			11,     // PIN_GPS_RX
			10,     // PIN_GPS_TX
			53,     // PIN_rESS
			55,		// PIN_GYRO
			30,     // PIN_RUDDER
			31,     // PIN_WINCH
		}

		Boat myBoat(&myProp);
	------------------------------*/

	/* Constructor. The boat requires a 'BoatProperties' so there is
	no default constructor. This constructor initializes all the
	necessary member variables. */
	Boat::Boat(BoatProperties * properties)
		: m_prop(properties), m_currentGPScoordinateIndex(0),
		m_searchPatternLatitude(nullptr), m_searchPatternLongitude(nullptr)
	{
		Serial.begin(19200);
		systemValidate();
	}

	/* Deconstructor. none of the methods or members in the boat
	are allocated on the heap and thus no cleanup is required when
	the boat object's stack frame is destroyed. */
	Boat::~Boat() {}

	bool Boat::systemValidate()
	{
		/* Printing of the set values. */
		Serial.println("----------------------------------------------------------");
		Serial.println("|                   Boat Properties                      |");
		Serial.println("----------------------------------------------------------");
		Serial.print("No Sailzone Angle:      "); Serial.println(m_prop->m_NoSailZoneAngle);
		Serial.print("Rudder Minimum:         "); Serial.println(m_prop->m_RudderMin);
		Serial.print("Rudder Maximum:         "); Serial.println(m_prop->m_RudderMin);
		Serial.print("Winch Minimum:          "); Serial.println(m_prop->m_winchMin);
		Serial.print("Winch Maximum:          "); Serial.println(m_prop->m_winchMax);
		Serial.print("GPS RX Pin:             "); Serial.println(m_prop->m_pin_GPS_RX);
		Serial.print("GPS TX Pin:             "); Serial.println(m_prop->m_pin_GPS_TX);
		Serial.print("Rotary Encoder Pin:     "); Serial.println(m_prop->m_pin_rESS);
		Serial.print("BNO055 Gyroscope Pin:   "); Serial.println(m_prop->m_pin_gyro);
		Serial.print("Rudder Pin:             "); Serial.println(m_prop->m_pin_rudder);
		Serial.print("Winch Pin:              "); Serial.println(m_prop->m_pin_winch);

		if (m_searchPatternLatitude)
		{
			Serial.println("----------------------------------------------------------");
			Serial.println("|               set GPS coordinate path                  |");
			Serial.println("----------------------------------------------------------");
			for (unsigned short i = 0; i < m_numSearchPatternCoordinates - 1; i++)
			{
				Serial.print("("); Serial.print(m_searchPatternLatitude[i]); Serial.print(", ");
				Serial.print(m_searchPatternLongitude[i]); Serial.print(")");
			}
		}
		else
			Serial.println("ALERT!: No search pattern has been set");
	}

	/* Method that calculates if the desired heading is an angle
	between the current wind angle's no sail zone range defined
	in 'm_prop'.
	 --Note: although this method returns a boolean, it actually
	modifies the member variables 'm_nszHigh' and 'm_nszLow'. */
	bool Boat::inNoSailZone()
	{
		double reAngle = rotaryEncoderInRadians();
		m_nszHigh = reAngle + m_prop->m_NoSailZoneAngle / 2;
		m_nszLow = reAngle - m_prop->m_NoSailZoneAngle / 2;
		return m_nszHigh > GPSsearchPatternDesiredHeading() > m_nszLow;
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
		digitalWrite(m_prop->m_pin_rESS, LOW);
		unsigned int value = SPI.transfer(0xFF);
		/* Bit shift left the bits of 'value'. */
		value << 8;
		/* Bitwise OR statement between 'value' and the return
		value of SPI.transfer.(0xFF). the OR statement turns
		all bits that are on *in either of them* on in the result.*/
		value | SPI.transfer(0xFF);
		/* Bit shift right the bits of 'value'*/
		value >> 5;
		/* Write to the rotary encoder's pin a value of 1. */
		digitalWrite(m_prop->m_pin_rESS, HIGH);
		/* Free the SPI bus. */
		SPI.end();
		return value;
	}

	/* Method that converts the boat's rotary encoder's
	value to an angle that is in radians. */
	inline double Boat::rotaryEncoderInRadians()
	{
		/* rotaryEncoderValueInRadians() returns an unsigned integer with
		value from 0 to 1023, so we divide it by its 'maximum value'
		of 1024 (its basically like saying 1023 is 99.9% around the
		circle and then 1024 is just back to the start, so it outputs
		0). we then multiply it by TAU or 2*PI to map it from 0 to 6.28.*/
		return (double)this->rotaryEncoderValue() / 1024 * TAU;
	}

	/* Method that returns the distance between two
	GPS coordinates whilst assuming the Earth is a
	cartesian plane on which x is longitude and
	y is latitude. */
	inline double Boat::GPScoordinateDistance()
	{
		/* Using the pythagorean theorem to get the distance between two points
		on a cartesian plane since arc length calculations are too computationally
		intesive for what difference they make on the scale of 500 feet.*/
		return sqrt(GPScoordinateDiffLong() * GPScoordinateDiffLong() + GPScoordinateDiffLat() * GPScoordinateDiffLat()) * 364610.4;
	}

	/* Method that returns the difference between the
	currently assigned search pattern longitude and the
	GPS's current longitude. */
	inline double Boat::GPScoordinateDiffLong()
	{
		if (m_searchPatternLongitude && m_currentGPScoordinateIndex < m_numSearchPatternCoordinates)
			return m_searchPatternLongitude[m_currentGPScoordinateIndex] - m_gps.location.lng();
		else
			return 100000.0;
	}

	/* Method that returns the difference between the
	currently assigned search pattern latitude and the
	GPS's current latitude. */
	inline double Boat::GPScoordinateDiffLat()
	{
		if (m_searchPatternLatitude && m_currentGPScoordinateIndex < m_numSearchPatternCoordinates)
			return m_searchPatternLatitude[m_currentGPScoordinateIndex] - m_gps.location.lat();
		else
			return 100000.0;
	}

	/* Method that returns the desired angle at which the boat
	should be heading (in radians) based on the currently selected
	point in the search pattern with respect to theta = 0 -> east. */
	inline double Boat::GPSsearchPatternDesiredHeading()
	{
		return atan2(GPScoordinateDiffLat(), GPScoordinateDiffLong());
	}

	/* Method that returns the desired angle at which the boat
	should be heading (in radians) based on the currently selected
	point in the search pattern with respect to its current heading. */
	inline double Boat::GPSsearchPatternDiffHeading()
	{
		return GPSsearchPatternDesiredHeading() - gyroCurrentHeading();
	}

	/* Method that returns the analog value at which the rudder should
	be set to in order to turn to the desired heading.
	 --Note: this function upon returning a value undergoes an implicit
	 conversion of double to int. */
	inline int Boat::desiredRudderValue()
	{
		return (GPSsearchPatternDiffHeading() - PI) / TAU * (m_prop->m_RudderMax - m_prop->m_RudderMin) + m_prop->m_RudderMin;
	}

	/* Method that returns the analog value at which the winch should
	be set to in order to open or bring in the sails.
	 --Note: this function upon returning a value undergoes an implicit
	 conversion of double to int. */
	inline int Boat::desiredWinchValue()
	{
		double angle = rotaryEncoderInRadians();
		if (angle > PI)
			angle = TAU - angle;
		return angle / TAU * (m_prop->m_winchMax - m_prop->m_winchMin) + m_prop->m_winchMin;
	}

	/* Method that returns what the bno055 electrical
	gyroscope reads for its roll.
	(orientation.v[0], orientation.x or orientation.roll) */
	double Boat::gyroCurrentHeading()
	{
		/* sensors_event_t is an object that m_gyro.getEvent() alters
		by assigning the current readings of the sensor to the passed
		object's member variables. */
		sensors_event_t reading;
		m_gyro.getEvent(&reading);
		/* Return the roll component. */
		return reading.orientation.roll;
	}

	/* Method that assigns the member variables pointing to
	two arrays of gps coordinates to a pointer that points to
	an actual array on the stack in the users scope. This method
	also searches for a 0 at the end to set the end of the search
	pattern as well as verify that the two pointers are pointing to
	arrays of equal length. */
	void Boat::setSearchPatternCoordinates(double * lat, double * lon, double acc)
	{
		m_acceptableRange = acc;
		while (lat[m_numSearchPatternCoordinates])
			m_numSearchPatternCoordinates++;
		/*---------------------
		|   bullet proofing   |
		---------------------*/
		if (lon[m_numSearchPatternCoordinates])
		{
			m_searchPatternLatitude = nullptr;
			m_searchPatternLongitude = nullptr;
		}
		else
		{
			m_searchPatternLatitude = lat;
			m_searchPatternLongitude = lon;
		}
	}

	/* Method that increments the current gps coordinate index.
	 --Note: call this method in loop for desired functionality. */
	void Boat::runThroughSearchPattern()
	{
		if (GPScoordinateDistance() < m_acceptableRange)
			m_currentGPScoordinateIndex++;
	}

	/* Method that calls all the necessary methods for the boat to sail.
	 --Note: call this method in loop for desired functionality. */
	void Boat::sail()
	{
		systemValidate();
		m_winchServo.writeMicroseconds(desiredWinchValue());
		m_rudderServo.writeMicroseconds(desiredRudderValue());
		if (m_searchPatternLatitude)
			runThroughSearchPattern();
	}

}
