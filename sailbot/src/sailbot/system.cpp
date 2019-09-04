#include "system.hpp"
#include "comm.hpp"
#include "timing.hpp"
#include <string.h>

namespace sailbot { namespace callbacks { namespace _internal {
    static void empty_void() {}
    static void (*on_data_read)() = empty_void;
    static void (*on_data_write)() = empty_void;
}}} // namespace sailbot::callbacks::_internal

namespace sailbot { namespace callbacks { namespace set {
    void on_data_read(void (*func)()) { callbacks::_internal::on_data_read = func; }
    void on_data_write(void (*func)()) { callbacks::_internal::on_data_write = func; }
}}} // namespace sailbot::callbacks::set

namespace sailbot { namespace system {
    static unsigned int s_current_port = 0;
    int init(const char *device_file, const unsigned int baud_rate) {
        for (unsigned int i = 0; i < sailbot::comm::PORT_COUNT; ++i)
            if (!strcmp(device_file, sailbot::comm::DEVICE_FILE[i])) s_current_port = i;
        const int result = sailbot::comm::open_device_err(s_current_port, baud_rate);
        clock::reset();
        return result;
    }
    static double s_current_time = 0, s_total_tick_time = 0;
    static unsigned long long s_total_tick_count = 0;
    bool update(void *tdata, const unsigned int tsize, void *rdata, const unsigned int rsize) {
        s_current_time = clock::now();
        if (s_current_time > s_total_tick_time + TICK_DURATION) {
            s_total_tick_count++;
            s_total_tick_time = s_current_time;
            if (s_total_tick_count % 2) {
                sailbot::comm::write_buffer(s_current_port, tdata, tsize);
                callbacks::_internal::on_data_write();
            } else {
                sailbot::comm::read_buffer(s_current_port, rdata, rsize);
                callbacks::_internal::on_data_read();
            }
        }
        return true;
    }
}} // namespace sailbot::system
