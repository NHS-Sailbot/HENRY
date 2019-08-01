#include "system.hpp"
#include "comm.hpp"
#include "timing.hpp"
#include <string>

namespace sailbot { namespace callbacks { namespace _internal {
    void empty_void_voidptr_const_uint(void *data, const unsigned int size) {}
    static void (*on_data_read)(void *data, const unsigned int size) = empty_void_voidptr_const_uint;
    static void (*on_data_write)(void *data, const unsigned int size) = empty_void_voidptr_const_uint;
}}} // namespace sailbot::callbacks::_internal

namespace sailbot { namespace callbacks { namespace set {
    void on_data_read(void (*func)(void *data, const unsigned int size)) { callbacks::_internal::on_data_read = func; }
    void on_data_write(void (*func)(void *data, const unsigned int size)) { callbacks::_internal::on_data_write = func; }
}}} // namespace sailbot::callbacks::set

namespace sailbot { namespace system {
    static unsigned int s_current_port = 0;
    int init(const char *device_file, const unsigned int baud_rate) {
        for (unsigned int i = 0; i < sailbot::comm::PORT_COUNT; ++i)
            if (!strcmp(device_file, sailbot::comm::DEVICE_FILE[i]))
                s_current_port = i;
        const int result = sailbot::comm::open_device_err(s_current_port, baud_rate);
        clock::reset();
        return result;
    }
    constexpr static double TICK_DURATION = 1.0 / 10;
    static double s_current_time = 0, s_total_tick_time = 0;
    static unsigned long long s_total_tick_count = 0;
    static bool s_transmission_switch = true;
    bool update(void *tdata, const unsigned int tsize, void *rdata, const unsigned int rsize) {
        s_current_time = clock::now();
        if (s_current_time > s_total_tick_time + TICK_DURATION) {
            s_total_tick_count++;
            s_total_tick_time = s_current_time;
            s_transmission_switch = s_total_tick_count % 2;
            if (s_transmission_switch) {
                sailbot::comm::write_buffer(s_current_port, tdata, tsize);
                callbacks::_internal::on_data_write(tdata, tsize);
            } else {
                sailbot::comm::read_buffer(s_current_port, rdata, rsize);
                callbacks::_internal::on_data_read(rdata, rsize);
            }
        }
        return true;
    }
}} // namespace sailbot::system
