#pragma once
#include "../../dep/math/src/math.hpp"

namespace sailbot { namespace tools {
    static constexpr double EARTH_MAJOR_RADIUS = 6378137.0, EARTH_MINOR_RADIUS = 6356752.3,
                            EARTH_RADIUS_RATIO = EARTH_MAJOR_RADIUS / EARTH_MINOR_RADIUS,
                            LATITUDE_TO_METERS = EARTH_MAJOR_RADIUS * math::TAU / 360.0,
                            LONGITUDE_TO_METERS = EARTH_MAJOR_RADIUS * math::TAU / 360.0;
    double gps_distance_meters(const math::dVec2 &g1, const math::dVec2 &g2);
}} // namespace sailbot::tools
