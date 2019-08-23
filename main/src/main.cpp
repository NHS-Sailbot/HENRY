#include <iostream>
#include <sailbot.hpp>

struct TData {                     // size is 6 bytes
    unsigned char m1_dir, m2_dir;  // 1 byte (uchar)
    unsigned short m1_pow, m2_pow; // 2 bytes (ushort)
};

struct RData {                    // size is 44 bytes
    float gps_lat, gps_lon;       // 4 bytes (float)
    float wind_direction;         // 4 bytes (float)
    float rc_switch;              // 4 bytes (float)
    math::Vec2 rc_left, rc_right; // 8 bytes (float pair)
    math::Vec3 magnetometer;      // 12 bytes (float triplet)
};

static TData tdata = {0, 1, 1024, 1024};
static RData rdata;

void tick() {
    std::cout << rdata.rc_left.x << ", " << rdata.rc_left.y << ", ";
    std::cout << rdata.rc_right.x << ", " << rdata.rc_right.y << "  |  ";
    std::cout << rdata.gps_lat << ", " << rdata.gps_lon << "  |  " << rdata.wind_direction << '\n';
}

int main() {
    sailbot::callbacks::set::on_data_read(tick);
    if (sailbot::system::init("/dev/ttyACM0", 115200))
        return 1;
    while (true) {
        sailbot::system::update(&tdata, sizeof(TData), &rdata, sizeof(RData));
    }
}
