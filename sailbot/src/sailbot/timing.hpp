#pragma once

namespace sailbot { namespace timing {
    void sleep(double seconds);
}} // namespace sailbot::timing

namespace sailbot { namespace clock {
    void reset();
    double now();
}} // namespace sailbot::clock
