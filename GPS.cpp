#include "GPS.h"



namespace HENRY
{

	GPS::GPS(unsigned char pin_rx, unsigned char pin_tx)
		: m_pinRX(pin_rx), m_pinTX(pin_tx), m_ss(pin_rx, pin_tx)
	{
		m_ss.begin(4800);
		/* a dedicated serial may be needed to be created
		on 115200 for this to work because I dont know if 
		thats what the other things operate on... I think
		the others are on 9600.*/
	}


	void GPS::update()
	{
		/* In the documentation I've read on the TinyGPS, this
		is needed for what they say is 'dispatching incoming
		characters', I interpret that as reformatting the 
		gps based on what comes in through the rx port. */
		while (m_ss.available())
			m_gps.encode(ss.read());
		/* call the method to check if the gps has changed 
		position. if it has, update the position variables
		in our wrapper class. */
		if (m_gps.location.isUpdated())
		{
			m_loc.x = m_gps.location.lng();
			m_loc.y = m_gps.location.lat();
		}
	}

}
