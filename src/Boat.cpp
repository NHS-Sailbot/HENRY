#include "Boat.h"
#include "GPS.h"
#include "Motor.h"
#include "Magnetometer.h"
#include "RotaryEncoder.h"
#include "RemoteControl.h"

namespace HENRY {
	namespace Boat {
		static constexpr const double TARGET_MIN_DIST = 10.0;
		static MotorData motor1 = {}, motor2 = {};
		static Math::dvec2 *s_trackPoints = nullptr;
		static ushort s_trackPointCount = 0, s_currentTrackPoint = 0;
		static byte s_isTrackingPoints = 0;

		void internal_Motor1Callback() { Motor::update(motor1); }
		void internal_Motor2Callback() { Motor::update(motor2); }

		uint init()
		{
			if (!Motor::init(motor1, 2, 3)) // ask jackson about pins
				return INIT::MOT_FAIL;
			attachInterrupt(2, internal_Motor1Callback, CHANGE); // ask about pin
			if (!Motor::init(motor2, 2, 3)) // ask jackson about pins
				return INIT::MOT_FAIL;
			attachInterrupt(2, internal_Motor2Callback, CHANGE); // ask about pin
			if (!RotaryEncoder::init()) // pins
				return INIT::ROT_FAIL;
			if (!GPS::init()) // serial pin
				return INIT::GPS_FAIL;
			if (!Magnetometer::init()) // pins
				return INIT::MAG_FAIL;
			if (!RemoteControl::init())
				return INIT::R_C_FAIL;
			return INIT::SUCCESS;
		}
		void update()
		{
			GPS::update();
			Motor::update(motor1);
			Motor::update(motor2);
			if (s_isTrackingPoints) {
				Math::dvec2 &point = s_trackPoints[s_currentTrackPoint],
							currentToDesired = point - GPS::getPos();
				// control motors depending on gps, magnetometer and rotary encoder

				// motor1.currPos;
				// Motor::setDir(motor1, HIGH);
				// Motor::setPow(motor1, 255);

				// GPS::getPos();

				// RotaryEncoder::getValue()

				// Magnetometer::getRot();

				// RemoteControl::getControlMode();
				// RemoteControl::getLeftVert();
				// RemoteControl::getLeftHoriz();

				if (currentToDesired.smag() < TARGET_MIN_DIST * TARGET_MIN_DIST) {
					s_currentTrackPoint++;
					if (s_currentTrackPoint > s_trackPointCount - 1)
						s_isTrackingPoints = false;
				}
			}
		}
		void setTargetRoute(Math::dvec2 *points, ushort pointCount)
		{
			if (!s_isTrackingPoints)
				s_isTrackingPoints = 1;

			s_trackPoints = points;
			s_trackPointCount = pointCount;
		}
	} // namespace Boat
} // namespace HENRY
