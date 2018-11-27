#pragma once

#include <SPI.h>
#include <Servo.h>
#include <TinyGPS++.h>
#include <Adafruit_BNO055.h>

#define NSZ_ANGLE            30
#define RUDDER_MIN            0
#define RUDDER_MAX         2100
#define WINCH_MIN             0
#define WINCH_MAX          2100
#define PIN_GPS_RX           11
#define PIN_GPS_TX           10
#define PIN_rESS             53
#define PIN_GYRO             55
#define PIN_RUDDER           30
#define PIN_WINCH            31

#define TAU 6.2831853071

namespace HENRY
{
	bool Ascii_ISO_3D(const char * data, const unsigned short width);
	
	struct BoatProperties
	{
		double m_NoSailZoneAngle;
		unsigned char m_pin_GPS_RX, m_pin_GPS_TX,
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
