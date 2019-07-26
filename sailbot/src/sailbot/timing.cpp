#include "timing.hpp"
#include <chrono>
#include <thread>

namespace sailbot { namespace timing {
    void sleep(double seconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(seconds * 1000)));
    }
}} // namespace sailbot::timing

namespace sailbot { namespace clock {
    std::chrono::time_point<std::chrono::steady_clock> start;
    void reset() { start = std::chrono::steady_clock::now(); }
    double now() {
        return static_cast<double>(
                   std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count()) /
               1000000;
    }
}} // namespace sailbot::clock
