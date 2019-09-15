#pragma once

namespace sailbot { namespace camera {
    static constexpr unsigned int width = 1280, height = 720;
    // Opens video device 0, using v4l2. Useful commands to know:
    //  - v4l2-ctl --list-formats-ext
    //  lists all supported resolutions and formats.
    //  - v4l2-ctl -d /dev/video0 -l
    //  lists the camera controls possible for the selected device
    bool open(const char *const device_filepath);
    // "Rotates" the 4 segmented shared buffer system to allow for
    // fast reads, and returns the last buffer in the queue, meaning
    // the data in the buffer pointed to by the return value is
    // around 4 frames behind.
    unsigned char *read();
    // Sets the streaming mode on the camera to off.
    void close();
}} // namespace sailbot::camera
