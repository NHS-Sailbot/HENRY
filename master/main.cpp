#include <arduino/arduino.hpp>
#include <camera/camera.hpp>
#include <debug/debug.hpp>

struct MainRData {
    unsigned int wind_sensor_reading;
};
struct MainTData {
    unsigned int m1_dir, m2_dir, m1_pow, m2_pow;
};
struct RemoteRData {
    unsigned short rc1, rc2, rc3, rc4, rc5, rc6;
};
struct RemoteTData {};

int main() {
    DEBUG_BEGIN_PROFILE_SESSION(Setup, "setup.json");

    auto main_ard = arduino::connect(sizeof(MainRData), sizeof(MainTData), 115200, 0xA5);
    auto remote_ard = arduino::connect(sizeof(RemoteRData), sizeof(RemoteTData), 115200, 0xBD);
    auto cam = camera::open(1280, 720);

    if (!remote_ard.is_valid) {
        debug::log::error("Unable to open remote arduino!");
        return -1;
    }
    if (!main_ard.is_valid) {
        debug::log::error("Unable to open main arduino!");
        return -1;
    }
    if (!cam.is_open) {
        debug::log::error("Unable to open camera!");
        return -1;
    }

    DEBUG_END_PROFILE_SESSION(Setup);
    return 0;
}
