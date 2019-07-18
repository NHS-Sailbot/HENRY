#include "timing.hpp"
#include <chrono>
#include <thread>

namespace sailbot { namespace timing {
    void sleep(double seconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(seconds * 1000)));
    }
}} // namespace sailbot::timing
