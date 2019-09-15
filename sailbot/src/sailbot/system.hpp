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
    struct Config {                                      // size: 12 bytes (32bit) 20 bytes (64bit)
        unsigned int baudrate;                           // size: 4 bytes
        char *ard_device_filepath, *cam_device_filepath; // size: 4 bytes (32bit) 8 bytes (64bit)
        Config() : baudrate(0), ard_device_filepath(nullptr), cam_device_filepath(nullptr) {}
        Config(const unsigned int baudrate) : baudrate(baudrate), ard_device_filepath(nullptr), cam_device_filepath(nullptr) {}
        Config(const unsigned int baudrate, char *const ard_device_filepath, char *const cam_device_filepath)
            : baudrate(baudrate), ard_device_filepath(ard_device_filepath), cam_device_filepath(cam_device_filepath) {}
        Config(const Config &config)
            : baudrate(config.baudrate), ard_device_filepath(config.ard_device_filepath),
              cam_device_filepath(config.cam_device_filepath) {}
        Config operator=(const Config &config) {
            baudrate = config.baudrate, ard_device_filepath = config.ard_device_filepath,
            cam_device_filepath = config.cam_device_filepath;
            return *this;
        }
    };

    // TODO: Documentation Comment
    int init(const Config &config);

    // TODO: Documentation Comment
    void update();

    // TODO: Documentation Comment
    bool should_close();

    // Returns the time inbetween the last time `sailbot::system::update()`
    // was called and the time that it was called before it.
    double get_elapsed_time();

    // TODO: Documentation Comment
    void shutdown();
}} // namespace sailbot::system
