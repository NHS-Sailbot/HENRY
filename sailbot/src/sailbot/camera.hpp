#pragma once

namespace sailbot { namespace camera {
    static constexpr unsigned int width = 800, height = 600;
    void open();
    unsigned char *read();
    void close();
}} // namespace sailbot::camera
