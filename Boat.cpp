#include "Boat.h"

#include <Wire.h>
#include <utility/imumaths.h>

namespace HENRY
{
	Boat::Boat(BoatProperties * prop)
		: m_prop(prop), m_isTrackingPoints(false), m_currentTrackPoint(0)
	{
	}

	void Boat::sailTowards(const Math::vec2& point)
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

	unsigned int Boat::init()
	{
		if (!bnoSensor.begin())
			return EXIT_CODE::BNO_FAIL;
		m_gps.init();
		delay(1000);
		bnoSensor.setExtCrystalUse(false);
	}

	void Boat::update()
	{
		bnoSensor.getEvent(&m_bnoEvent);
		m_gps.update();

		if (m_isTrackingPoints)
			sailTowards(*m_trackPoints[m_currentTrackPoint]);
	}

	Math::vec2& Boat::getPos()
	{
		return m_gps.getCoord();
	}

	Math::vec3& Boat::getRot()
	{
		return { m_bnoEvent.orientation.x, m_bnoEvent.orientation.y, m_bnoEvent.orientation.z };
	}

	void setTargetPos(const Math::vec2 &point)
	{
		if (!m_isTrackinPoints)
			m_isTrackingPoints = true;

		m_trackPoints = &point;
		m_trackPointCount = 1;
	}

	void setTargetRoute(const Math::vec2 *points, unsigned short pointCount)
	{
		if (!m_isTrackinPoints)
			m_isTrackingPoints = true;

		m_trackPoints = &point;
		m_trackPointCount = 1;
	}
}
