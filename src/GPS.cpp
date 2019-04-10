#include "GPS.h"

namespace HENRY
{
// TODO: Add usage documentation.

bool GPS::init()
{
	// Begin the debugging serial "Serial" on 9600
	// and the dedicated GPS Serial "Serial1" on 9600.
	// This is to be called in "setup()"
	Serial1.begin(9600);
	return true;
}

void GPS::update()
{
	// In the documentation I've read on the TinyGPS, this
	// is needed for what they say is 'dispatching incoming
	// characters', I interpret that as reformatting the
	// gps based on what comes in through the rx port.
	while (Serial1.available())
		m_gps.encode(Serial1.read());
	// call the method to check if the gps has changed
	// position. if it has, update the position variables
	// in our wrapper class.
	if (m_gps.location.isUpdated())
	{
		m_loc.x = m_gps.location.lng();
		m_loc.y = m_gps.location.lat();
	}
}
} // namespace HENRY
