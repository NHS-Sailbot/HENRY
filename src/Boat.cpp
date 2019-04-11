#include "Boat.h"

#define TARGET_MINIMUM_DIST 10

namespace HENRY
{
Boat::Boat(BoatProperties *prop)
	: m_prop(prop), m_isTrackingPoints(false), m_currentTrackPoint(0)
{
}

void Boat::sailTowards(Math::dvec2 &point)
{
	// figure out how to sail

	// test if close to current track point
	if ((point - getPos()).smag() < TARGET_MINIMUM_DIST * TARGET_MINIMUM_DIST)
	{
		m_currentTrackPoint++;
		if (m_currentTrackPoint > m_trackPointCount - 1)
			m_isTrackingPoints = false;
	}
}

unsigned char Boat::init()
{
	if (!m_mag.init())
		return INIT::MAG_FAIL;

	if (!m_rot.init(0, 0))
		return INIT::ROT_FAIL;

	if (!m_mot1.init(0, 0))
		return INIT::MOT_FAIL;

	if (!m_mot2.init(0, 0))
		return INIT::MOT_FAIL;

	if (!m_gps.init())
		return INIT::GPS_FAIL;

	return INIT::SUCCESS;
}

void Boat::update()
{
	m_mag.update();
	m_gps.update();

	if (m_isTrackingPoints)
		sailTowards(m_trackPoints[m_currentTrackPoint]);
}

void Boat::setTargetPos(Math::dvec2 &point)
{
	if (!m_isTrackingPoints)
		m_isTrackingPoints = true;

	m_trackPoints = &point;
	m_trackPointCount = 1;
}

void Boat::setTargetRoute(Math::dvec2 *points, unsigned short pointCount)
{
	if (!m_isTrackingPoints)
		m_isTrackingPoints = true;

	m_trackPoints = points;
	m_trackPointCount = pointCount;
}
} // namespace HENRY
