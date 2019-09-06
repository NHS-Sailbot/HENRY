#include <sailbot.hpp>
#include <stdio.h>

#include "shaders.hpp"

struct TData {                     // size is 6 bytes
    unsigned char m1_dir, m2_dir;  // 1 byte (uchar)
    unsigned short m1_pow, m2_pow; // 2 bytes (ushort)
};

struct RData {                    // size is 44 bytes
    float gps_lat, gps_lon;       // 4 bytes (float)
    float wind_direction;         // 4 bytes (float)
    float rc_switch;              // 4 bytes (float)
    math::Vec2 rc_left, rc_right; // 8 bytes (float pair)
    math::Vec3 magnetometer;      // 12 bytes (float triplet)
};

static TData tdata = {0, 1, 1024, 1024};
static RData rdata;

static unsigned int frames_per_tick = 0;

void tick() {
    // printf("%f, %f, ", rdata.rc_left.x, rdata.rc_left.y);
    // printf("%f, %f | ", rdata.rc_right.x, rdata.rc_right.y);
    // printf("%f, %f | %f\n", rdata.gps_lat, rdata.gps_lat, rdata.wind_direction);
    printf("fps: %f\n", 1.0 / sailbot::system::TICK_DURATION * frames_per_tick);
    frames_per_tick = 0;
}

int main() {
    if (!sailbot::camera::open()) return -1;

    sailbot::callbacks::set::on_data_read(tick);
    sailbot::system::init("/dev/ttyACM0", 115200);

    coel::Window window(sailbot::camera::width, sailbot::camera::height, "window");
    coel::Shader shader(shaders::vert_src, shaders::frag_src);
    coel::Texture texture(sailbot::camera::width, sailbot::camera::height, coel::ColorSpace::RGB);

    coel::renderer::batch2d::init();

    while (!window.should_close()) {
        sailbot::system::update(&tdata, sizeof(tdata), &rdata, sizeof(rdata));

        unsigned char image_data[sailbot::camera::width * sailbot::camera::height * 3];
        unsigned char *buffer_start = sailbot::camera::read();
        if (buffer_start == nullptr) {
            // printf(".");
            buffer_start = image_data;
        }
        unsigned char *cam_y = buffer_start, *cam_u = buffer_start + 1, *cam_v = buffer_start + 3;
        for (unsigned int y = 0; y < sailbot::camera::height; ++y) {
            for (unsigned int x = 0; x < sailbot::camera::width; ++x) {
                const unsigned int index = x * 3 + y * sailbot::camera::width * 3;
                image_data[index + 0] = *cam_y;
                image_data[index + 1] = *cam_u;
                image_data[index + 2] = *cam_v;
                if (index % 2) { cam_u += 4, cam_v += 4; }
                cam_y += 2;
            }
        }
        texture.update(image_data);

        coel::renderer::batch2d::submit_rect(-1, 1, 2, -2);
        coel::renderer::batch2d::flush();

        window.update();
        frames_per_tick++;
    }
}
