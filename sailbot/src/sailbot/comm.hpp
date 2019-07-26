#pragma once

namespace sailbot { namespace comm {
    int open_device(const unsigned int port, const unsigned int baudrate);
    int open_device_err(const unsigned int port, const unsigned int baudrate);
    void write_buffer(const unsigned int port, const void *data, const unsigned int size);
    void read_buffer(const unsigned int port, void *data, const unsigned int size);
}} // namespace sailbot::comm
