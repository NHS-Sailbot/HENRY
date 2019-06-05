#include "Boat.h"
#include "GPS.h"
#include "Motor.h"
#include "Magnetometer.h"
#include "RotaryEncoder.h"
#include "RemoteControl.h"

constexpr static const float NSZ_RADIANS = PI / 6;

namespace HENRY {
	namespace Boat {
		static constexpr const double TARGET_MIN_DIST_IN_DEGREES = 0.01;
		static MotorData motor1 = {};
		static Servo servo1;
		// sailThroughCourse()
		static Math::dvec2 *s_trackPoints = nullptr;
		static ushort s_trackPointCount = 0, s_currentTrackPoint = 0;
		static byte s_isTrackingPoints = 0;
		// stationKeep();
		static int s_restartTimer = 1, s_keepStationKeeping = 1;
		static unsigned long s_millisTimerStart = 0;
		// RC
		static unsigned long s_millisDebugOffset = 0;

		void internal_Motor1Callback() { Motor::update(motor1); }

		static int sailTowards(Math::dvec2 point)
		{
			Math::dvec2 currentToDesired = point - GPS::getPos();
			float windAngleRelativeToTheHeading = RotaryEncoder::getValue(),
				  desiredAngleOfSailsRelativeToTheHeading;
			Math::dvec3 rot = Magnetometer::getRot();

			// Set the desired Sail angle
			if (windAngleRelativeToTheHeading > PI)
				desiredAngleOfSailsRelativeToTheHeading = 2 * PI - windAngleRelativeToTheHeading;
			desiredAngleOfSailsRelativeToTheHeading -= NSZ_RADIANS;
			if (desiredAngleOfSailsRelativeToTheHeading < 0)
				desiredAngleOfSailsRelativeToTheHeading = 0;

			// control motor to set out the sails

			Motor::setAngle(motor1, desiredAngleOfSailsRelativeToTheHeading);

			// Get desired sailing angle relative to the boat
			float absoluteDesiredAngleToTurnBoat = atan2(currentToDesired.y, currentToDesired.x);
			if (absoluteDesiredAngleToTurnBoat < PI)
				absoluteDesiredAngleToTurnBoat += TWO_PI;
			float absoluteBoatHeading = atan2(sin(rot.x), cos(rot.x)), relativeDesiredAngleToTurn;
			if (absoluteBoatHeading < PI)
				absoluteBoatHeading += TWO_PI;

			relativeDesiredAngleToTurn = absoluteBoatHeading - absoluteDesiredAngleToTurnBoat;

			// control servo to turn the rudder

			if (relativeDesiredAngleToTurn > 0)
				servo1.write(0);
			else
				servo1.write(180);
			if (currentToDesired.smag() < TARGET_MIN_DIST_IN_DEGREES * TARGET_MIN_DIST_IN_DEGREES) {
				return 1;
			}
			return 0;
		}
		static void sailThroughCourse()
		{
			if (s_isTrackingPoints) {
				Math::dvec2 &point = s_trackPoints[s_currentTrackPoint];
				if (sailTowards(point)) {
					s_currentTrackPoint++;
					if (s_currentTrackPoint > s_trackPointCount - 1)
						s_isTrackingPoints = false;
				}
			}
		}
		static void stationKeep()
		{
			if (s_keepStationKeeping) {
				Math::dvec2 &point = s_trackPoints[s_currentTrackPoint];
				if (sailTowards(point)) {
					s_restartTimer = 0;
					s_millisTimerStart = millis();
				}
				if (millis() - s_millisTimerStart > 5 * 1000 * 60)
					s_keepStationKeeping = 0;
			}
		}
		static void remoteControlBoat()
		{
			float leftRC = RemoteControl::getLeftStick();
			float rightRC = RemoteControl::getRightStick();
			if (millis() - s_millisDebugOffset > 1000) {
				s_millisDebugOffset = millis();
				Serial.print("Left: ");
				Serial.print(leftRC);
				Serial.print(", Right: ");
				Serial.println(rightRC);
			}
			if (leftRC < 0.4) {
				// left
				servo1.write(0);
			} else if (leftRC > 0.6) {
				// middle
				servo1.write(180);
			} else {
				// center
				servo1.write(90);
			}
			if (rightRC < 0.4) {
				// left
				// Motor::setAngle(motor1, 0);
			} else if (rightRC > 0.6) {
				// middle
				// Motor::setAngle(motor1, 3.14);
			} else {
				// center
				// Motor::setAngle(motor1, 0);
			}
		}

		uint init()
		{
			if (!Motor::init(motor1, 6, 7)) // winch pins
				return INIT::MOT_FAIL;
			attachInterrupt(2, internal_Motor1Callback, CHANGE); // winch pins
			servo1.attach(45); // rudder pin
			if (!RotaryEncoder::init()) // pins
				return INIT::ROT_FAIL;
			if (!GPS::init()) // serial pin
				return INIT::GPS_FAIL;
			if (!Magnetometer::init()) // pins
				return INIT::MAG_FAIL;
			if (!RemoteControl::init()) // pins
				return INIT::R_C_FAIL;
			return INIT::SUCCESS;
		}
		void update()
		{
			GPS::update();
			Motor::update(motor1);

			// remoteControlBoat();
			// if (RemoteControl::getControlMode() == RemoteControl::Mode::Auto) {
			// 	stationKeep();
			// } else if (RemoteControl::getControlMode() == RemoteControl::Mode::Remote) {
			// } else { // Control mode off
			// }
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
