#include "timing.hpp"
#include <chrono>
#include <thread>

namespace sailbot { namespace timing {
    void sleep(double seconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(seconds * 1000.0)));
    }
}} // namespace sailbot::timing

namespace sailbot { namespace clock {
    using namespace std::chrono;
    static time_point<steady_clock> start;
    void reset() { start = steady_clock::now(); }
    double now() { return duration_cast<microseconds>(steady_clock::now() - start).count() / 1000000.0; }
}} // namespace sailbot::clock
