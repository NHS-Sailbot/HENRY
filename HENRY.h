#pragma once

#include <SPI.h>
#include <Servo.h>
#include <TinyGPS++.h>
#include <Adafruit_BNO055.h>

#define TAU 6.2831853071

namespace HENRY
{
	bool Ascii_ISO_3D(const char * data, const unsigned short width);
	
	struct BoatProperties
	{
		double m_NoSailZoneAngle;
		unsigned short m_RudderMin, m_RudderMax,
			m_winchMin, m_winchMax,
			m_pin_GPS_RX, m_pin_GPS_TX,
			m_pin_rESS,
			m_pin_gyro,
			m_pin_rudder,
			m_pin_winch;
	};
	
	class Boat
	{
	private:
		BoatProperties * m_prop;
		unsigned short m_currentGPScoordinateIndex;
		unsigned short m_numSearchPatternCoordinates;
		double * m_searchPatternLongitude;
		double * m_searchPatternLatitude;
		double m_nszHigh, m_nszLow;
		double m_acceptableRange;
		Servo m_rudderServo, m_winchServo;
		Adafruit_BNO055 m_gyro;
		TinyGPSPlus m_gps;
	public:
		Boat(BoatProperties * properties);
		~Boat();
		bool systemValidate();
		bool inNoSailZone();
		unsigned int rotaryEncoderValue();
		inline double rotaryEncoderInRadians();
		inline double GPScoordinateDistance();
		inline double GPScoordinateDiffLong();
		inline double GPScoordinateDiffLat();
		inline double GPSsearchPatternDesiredHeading();
		inline double GPSsearchPatternDiffHeading();
		inline int desiredRudderValue();
		inline int desiredWinchValue();
		double gyroCurrentHeading();
		void setSearchPatternCoordinates(double * lat, double * lon, double acc);
		void runThroughSearchPattern();
		void sail();
	};
}
