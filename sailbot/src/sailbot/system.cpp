#include "comm.hpp"
#include "timing.hpp"
#include <iostream>

namespace sailbot { namespace system {
    constexpr unsigned int PORT = 2, BAUD_RATE = 57600;
    int init() {
        const int result = sailbot::comm::open_device(PORT, BAUD_RATE);
        clock::reset();
        return result;
    }
    bool update() { return true; }
    constexpr static double TICK_DURATION = 1.0 / 10;
    static double s_current_time = 0, s_total_tick_time = 0;
    static unsigned long long s_total_tick_count = 0;
    static bool s_transmission_switch = true;
    bool should_tick() {
        s_current_time = clock::now();
        if (s_current_time > s_total_tick_time + TICK_DURATION) {
            s_total_tick_count++;
            s_total_tick_time = s_current_time;
            s_transmission_switch = s_total_tick_count % 2;
            return true;
        } else
            return false;
    }
    void handshake(const void *tdata, const unsigned int tsize, void *rdata, const unsigned int rsize) {
        if (s_transmission_switch)
            sailbot::comm::write_buffer(PORT, tdata, tsize);
        else
            sailbot::comm::read_buffer(PORT, rdata, rsize);
    }
}} // namespace sailbot::system
