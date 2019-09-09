#pragma once

namespace sailbot { namespace callbacks { namespace set {
    // Sets the callback function that is called whenever the
    // system ticks on an even interval.
    // This function takes a `void (*func_name)()`
    // which is a pointer to a function that has no
    // arguments and doesn't return a value.
    void on_data_read(void (*func)());

    // Sets the callback function that is called whenever the
    // system ticks on an odd interval.
    // This function takes a `void (*func_name)()`
    // which is a pointer to a function that has no
    // arguments and doesn't return a value.
    void on_data_write(void (*func)());
}}} // namespace sailbot::callbacks::set

namespace sailbot { namespace system {
    // TODO: Documentation Comment
    constexpr static double TICK_DURATION = 1.0 / 1;

    // Initializes the sailbot system by opening the specified
    // device file in the Operating System to allow for data
    // transfer over serial to the Arduino.
    // This function takes a `const char *device_file` and
    // a `const unsigned int baud_rate`.
    int init(const char *device_file, const unsigned int baud_rate);

    // Updates the data pointed to in the first and third
    // arguments by sending and recieving data over the serial
    // port specified in `sailbot::system::init(...)`.
    // This function takes a `void *tdata`, a `const unsigned int tsize`,
    // a `void *rdata` and a `const unsigned int rsize`, representing
    // the data buffers to update on either end.
    // (t means transmit and r means recieve)
    bool update(void *tdata, const unsigned int tsize, void *rdata, const unsigned int rsize);

    // Returns the time inbetween the last time `sailbot::system::update()`
    // was called and the time that it was called before it.
    double get_elapsed_time();
}} // namespace sailbot::system
