#pragma once

#define NSZ_ANGLE            30
#define RUDDER_MAX         2100
#define RUDDER_MIN            0
#define WINCH_MAX          2100
#define WINCH_MIN             0
#define PIN_GPS_RX           11
#define PIN_GPS_TX           10
#define PIN_rESS             53
#define PIN_GYRO             55
#define PIN_RUDDER           30
#define PIN_WINCH            31

namespace HENRY
{
	bool Ascii_ISO_3D(const char * data, const unsigned short width);
	
	struct BoatProperties
	{
		unsigned short m_NoSailZoneAngle,
			m_RudderMax, m_RudderMin,
			m_winchMax, m_winchMin,
			m_pin_GPS_TX, m_pin_GPS_RX,
			m_pin_rESS,
			m_pin_gyro,
			m_pin_rudder,
			m_pin_winch;
	};
		
	class Boat
	{
	private:
		BoatProperties * m_prop;
		unsigned short currentGPScoordinateIndex;
		unsigned short m_numSearchPatternCoordinates;
		double * m_searchPatternLongitude;
		double * m_searchPatternLatitude;
		double m_nszHigh, m_nszLow;
		TinyGPSPlus m_gps;
	public:
		Boat(BoatProperties * properties);
		~Boat();
		bool systemValidate();
		unsigned int rotaryEncoderValue();
		inline double rotaryEncoderInRadians();
		inline double GPScoordinateDistance();
		inline double GPScoordinateDiffLong();
		inline double GPScoordinateDiffLat();
		inline double GPSsearchPatternDesiredHeading();
		inline double GPSsearchPatternDiffHeading();
		double gyroCurrentHeading();
		bool inNoSailZone();
	};	
}
