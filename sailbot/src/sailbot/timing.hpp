#pragma once

namespace sailbot { namespace timing {
    // Sleeps the current thread for the specified
    // number of seconds.
    // This function takes a `double seconds`
    void sleep(double seconds);
}} // namespace sailbot::timing

namespace sailbot { namespace clock {
    // Resets the timer, so that when `sailbot::clock::now()`
    // is called, it returns the time since the last reset.
    void reset();

    // Returns the time since the last time `sailbot::clock::reset()`
    // was called, or the time since the program started.
    double now();
}} // namespace sailbot::clock
