#include <iostream>
#include <sailbot.hpp>

struct TData {
    unsigned char m1_dir, m2_dir;
    unsigned int m1_pow, m2_pow;
};

struct RData {
    math::Vec2 gps, rc_left, rc_right;
    float wind_direction;
};

TData tdata = {0, 1, 1024, 1024};
RData rdata;

int main() {
    if (sailbot::system::init("\\\\.\\COM3", 57600))
        return 1;

    while (sailbot::system::update(&tdata, sizeof(TData), &rdata, sizeof(RData))) {
        std::cout << rdata.rc_left.x << ", " << rdata.rc_left.y << ", ";
        std::cout << rdata.rc_right.x << ", " << rdata.rc_right.y << "  |  ";
        std::cout << rdata.gps.y << ", " << rdata.gps.x << "  |  " << rdata.wind_direction << '\n';
    }
}
