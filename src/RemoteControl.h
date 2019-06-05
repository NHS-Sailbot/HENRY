#pragma once

namespace HENRY {
	namespace RemoteControl {
		enum Mode {
			Off,
			Remote,
			Auto
		};
		int init();
		float getLeftStick();
		float getRightStick();
		int getControlMode();
	} // namespace RemoteControl
} // namespace HENRY