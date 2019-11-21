#include <chrono>

namespace debug { namespace timer {
    using namespace std::chrono;
    static time_point<steady_clock> _start = steady_clock::now();
    double now() { return duration_cast<microseconds>(steady_clock::now() - _start).count() / 1000000.0; }
    void reset() { _start = steady_clock::now(); }
    void sleep(const double s) {
        const double start = now();
        while (now() - start < s) {
            // do nothing
        }
    }
}} // namespace debug::timer
