#pragma once

namespace sailbot { namespace callbacks { namespace set {
    void on_data_read(void (*func)(void *data, const unsigned int size));
    void on_data_write(void (*func)(void *data, const unsigned int size));
}}} // namespace sailbot::callbacks::set

namespace sailbot { namespace system {
    int init(const char *device_file, const unsigned int baud_rate);
    bool update(void *tdata, const unsigned int tsize, void *rdata, const unsigned int rsize);
}} // namespace sailbot::system
