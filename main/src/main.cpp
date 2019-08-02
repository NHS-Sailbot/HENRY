#include <iomanip>
#include <iostream>
#include <sailbot.hpp>

struct TData {
    unsigned char m1_dir, m2_dir;
    unsigned int m1_pow, m2_pow;
};

struct RData {
    long long gps_lat, gps_lon;
    math::Vec2 rc_left, rc_right;
    math::Vec3 mag;
    float wind_direction;
};

math::dVec2 s_last_gps = {42.814441, -70.886167};
double s_dist_since_last_tick = 0;

void tick(void *p_data, const unsigned int size) {
    RData data = *reinterpret_cast<RData *>(p_data);
    math::dVec2 current_gps = {static_cast<double>(data.gps_lat) / 1000000, static_cast<double>(data.gps_lon) / 1000000};
    s_dist_since_last_tick = sailbot::tools::gps_distance_meters(current_gps, s_last_gps);
    s_last_gps = current_gps;

    std::cout << std::setw(10) << s_dist_since_last_tick << "  ";
    std::cout << data.rc_left.x << ", " << data.rc_left.y << ", ";
    std::cout << data.rc_right.x << ", " << data.rc_right.y << "  |  ";
    std::cout << data.gps_lat << ", " << data.gps_lon << "  |  " << data.wind_direction << '\n';
}

int main() {
    if (sailbot::system::init("\\\\.\\COM3", 115200))
        return 1;
    sailbot::callbacks::set::on_data_read(tick);

    TData tdata = {0, 1, 1024, 1024};
    RData rdata;

    while (sailbot::system::update(&tdata, sizeof(TData), &rdata, sizeof(RData))) {
    }
}
