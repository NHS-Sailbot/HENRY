#include "system.hpp"
#include "camera.hpp"
#include "comm.hpp"
#include "timing.hpp"

#include "../dep/math/src/math.hpp"
#include "coel.hpp"

#include <stdio.h>
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
    struct PixelRGB {
        char r, g, b;
    };
    static PixelRGB camera_data_rgb[camera::width * camera::height];
    static unsigned char *camera_data_raw;
    static Config s_config;

    static coel::Window s_window;
    static coel::Texture s_texture;

    static constexpr char *vert_src = R"(
        #version 330 core
        layout(location = 0) in vec2 pos;
        layout(location = 1) in vec2 tex;
        layout(location = 2) in vec4 col;
        layout(location = 3) in float tid;

        out vec2 v_tex;

        void main() {
            v_tex = tex;
            gl_Position = vec4(pos, 0, 1);
})";

    static constexpr char *frag_src = R"(
        #version 330 core
        in vec2 v_tex;

        uniform sampler2D tex;

        out vec4 color;

        vec4 yuv422_rgb(in vec4 col) {
            vec4 result;
            int c = int(col.x * 256 - 16);
            int d = int(col.y * 256 - 128);
            int e = int(col.z * 256 - 128);
            return vec4(
                clamp(float((298 * c + 409 * e + 128) >> 8) / 256, 0, 256),
                clamp(float((298 * c - 100 * d - 208 * e + 128) >> 8) / 256, 0, 256),
                clamp(float((298 * c + 516 * d + 128) >> 8) / 256, 0, 256),
                1.0
            );
        }

        void main() {
            color = yuv422_rgb(texture(tex, v_tex));
})";

    static inline void run_through_arduino_device_files() {
        printf("Searching for arduino...\n");
        char *const filepath = "/dev/ttyACM0";
        while (!arduino::connect(filepath, s_config.baudrate) && filepath[11] < '9' + 1) {
            printf("unable to open `%s`\n", filepath);
            ++filepath[11];
        }
        printf("opened `%s`\n", filepath);
    }

    static inline void run_through_camera_device_files() {
        printf("Searching for camera...\n");
        char *const filepath = "/dev/video0";
        while (!camera::open(filepath) && filepath[10] < '9' + 1) {
            printf("unable to open `%s`\n", filepath);
            ++filepath[10];
        }
        printf("opened `%s`\n", filepath);
    }

    int init(const Config &config) {
        s_config = config;

        if (!config.ard_device_filepath)
            run_through_arduino_device_files();
        else if (!arduino::connect(config.ard_device_filepath, config.baudrate)) {
            printf("unable to open specified file: `%s`\n", config.ard_device_filepath);
            run_through_arduino_device_files();
        }

        if (!config.cam_device_filepath)
            run_through_camera_device_files();
        else if (!camera::open(config.cam_device_filepath)) {
            printf("unable to open specified file: `%s`\n", config.cam_device_filepath);
            run_through_camera_device_files();
        }

        s_window.init(camera::width, camera::height, "sailbot window");
        coel::renderer::batch2d::init();
        s_texture.init(sailbot::camera::width, sailbot::camera::height, coel::ColorSpace::RGB);
        // TODO: Add compute shader to process the pixel transform
        coel::Shader shader(vert_src, frag_src);
        return 1;
    }

    static constexpr double S_TICK_DURATION = 0.5;
    static unsigned long long s_total_tick_count = 0;
    static double s_current_time = 0, s_previous_time = 0, s_elapsed_time = 0, s_total_tick_time = 0;

    struct TData {
        unsigned char m1_dir, m2_dir;
        unsigned short m1_pow, m2_pow;
    } s_tdata;

    struct RData {
        float gps_lat, gps_lon;
        float wind_direction;
        float rc_switch;
        math::Vec2 rc_left, rc_right;
        math::Vec3 magnetometer;
    } s_rdata;

    void update() {
        // Tick system
        s_current_time = clock::now();
        s_elapsed_time = s_current_time - s_previous_time;
        s_previous_time = s_current_time;
        if (s_current_time > s_total_tick_time + S_TICK_DURATION) {
            s_total_tick_count++;
            s_total_tick_time = s_current_time;
            if (s_total_tick_count % 2) {
                arduino::transmit(&s_tdata, sizeof(TData));
                callbacks::_internal::on_data_write();
            } else {
                arduino::receive(&s_rdata, sizeof(RData));
                callbacks::_internal::on_data_read();
            }
        }

        // Display updates
        camera_data_raw = sailbot::camera::read();
        if (camera_data_raw == nullptr) camera_data_raw = reinterpret_cast<unsigned char *>(camera_data_rgb);
        unsigned char *cam_y = camera_data_raw, *cam_u = camera_data_raw + 1, *cam_v = camera_data_raw + 3;
        for (unsigned int i = 0; i < camera::width * camera::height; ++i) {
            camera_data_rgb[i] = {*cam_y, *cam_u, *cam_v};
            if (i % 2) { cam_u += 4, cam_v += 4; }
            cam_y += 2;
        }
        s_texture.update(camera_data_rgb);

        coel::renderer::clear();
        coel::renderer::batch2d::submit_rect(-1, 1, 2, -2);
        coel::renderer::batch2d::flush();

        s_window.update();
    }

    bool should_close() { return s_window.should_close(); }

    double get_elapsed_time() { return s_elapsed_time; }

    void shutdown() {
        arduino::disconnect();
        camera::close();
    }
}} // namespace sailbot::system
