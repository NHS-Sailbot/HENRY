#include "tools.hpp"
#include <math.h>

namespace sailbot { namespace tools {
    double gps_distance_meters(const math::dVec2 &g1, const math::dVec2 &g2) {
        const double dx = g1.y - g2.y, dy = g1.x - g2.x, avg_y = (g1.x + g2.x) / 2.0,
                     sin_avg = math::sin(avg_y * math::DEG_TO_RAD), cos_avg = math::cos(avg_y * math::DEG_TO_RAD),
                     oblique_scalar = sqrt(EARTH_RADIUS_RATIO * EARTH_RADIUS_RATIO * cos_avg * cos_avg + sin_avg * sin_avg),
                     dx_m = dx * LONGITUDE_TO_METERS * cos_avg, dy_m = dy * LATITUDE_TO_METERS * oblique_scalar;
        return sqrt(dx_m * dx_m + dy_m * dy_m);
    }
}} // namespace sailbot::tools
