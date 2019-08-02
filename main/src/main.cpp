#include <cmath>
#include <iostream>
#include <sailbot.hpp>

static constexpr double EARTH_MAJOR_RADIUS = 6378137.0, EARTH_MINOR_RADIUS = 6356752.3,
                        EARTH_RADIUS_RATIO = EARTH_MAJOR_RADIUS / EARTH_MINOR_RADIUS,
                        LATITUDE_TO_METERS = EARTH_MAJOR_RADIUS * math::TAU / 360.0,
                        LONGITUDE_TO_METERS = EARTH_MAJOR_RADIUS * math::TAU / 360.0;

static double gps_distance_meters(const math::dVec2 &g1, const math::dVec2 &g2) {
    const double dx = g1.x - g2.x, dy = g1.y - g2.y, avg_y = (g1.y + g2.y) / 2.0,
                 sin_avg_y = math::sin(avg_y * math::DEG_TO_RAD), cos_avg_y = math::cos(avg_y * math::DEG_TO_RAD),
                 oblique_scalar = sqrt(EARTH_RADIUS_RATIO * EARTH_RADIUS_RATIO * cos_avg_y * cos_avg_y + sin_avg_y * sin_avg_y),
                 dx_m = dx * LONGITUDE_TO_METERS * cos_avg_y, dy_m = dy * LATITUDE_TO_METERS * oblique_scalar;
    return sqrt(dx_m * dx_m + dy_m * dy_m);
}

struct TData {
    unsigned char m1_dir, m2_dir;
    unsigned int m1_pow, m2_pow;
};

struct RData {
    math::Vec2 gps, rc_left, rc_right;
    float wind_direction;
};

math::dVec2 s_last_gps = {0, 0};
double s_dist_since_last_tick = 0;

void tick(void *p_data, const unsigned int size) {
    RData data = *reinterpret_cast<RData *>(p_data);
    math::dVec2 current_gps = {data.gps.x, data.gps.y};
    s_dist_since_last_tick = gps_distance_meters(current_gps, s_last_gps);
    s_last_gps = current_gps;

    std::cout << s_dist_since_last_tick << "        ";
    std::cout << data.rc_left.x << ", " << data.rc_left.y << ", ";
    std::cout << data.rc_right.x << ", " << data.rc_right.y << "  |  ";
    std::cout << data.gps.y << ", " << data.gps.x << "  |  " << data.wind_direction << '\n';
}

int main() {
    if (sailbot::system::init("/dev/ttyACM1", 57600))
        return 1;
    sailbot::callbacks::set::on_data_read(tick);

    TData tdata = {0, 1, 1024, 1024};
    RData rdata;

    while (sailbot::system::update(&tdata, sizeof(TData), &rdata, sizeof(RData))) {
    }
}
