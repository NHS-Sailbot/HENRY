#pragma once

namespace sailbot { namespace camera {
    static constexpr unsigned int width = 1280, height = 720;
    void open();
    void get_capability();
    void set_format();
    void set_framerate();
    void create_buffers();
    unsigned char *read();
    void close();
}} // namespace sailbot::camera
