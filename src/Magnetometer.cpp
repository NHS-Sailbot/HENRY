#include "Magnetometer.h"

namespace HENRY
{
Magnetometer::Magnetometer()
	: m_rot{0, 0, 0}
{
}

bool Magnetometer::init()
{
	if (!m_mag.begin())
		return false;
	delay(100);
	m_mag.setExtCrystalUse(false);
	return true;
}

void Magnetometer::update()
{
	sensors_event_t event;
	m_mag.getEvent(&event);
	m_rot = {event.orientation.x,
			 event.orientation.y,
			 event.orientation.z};
}
} // namespace HENRY
