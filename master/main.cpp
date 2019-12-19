#define DEBUG_ENABLE_TIMING
#include <debug/debug.hpp>

#include <arduino/arduino.hpp>
#include <camera/camera.hpp>
#include <coel/coel.hpp>

#include "shaders.hpp"

struct MainR {
    unsigned int wind_sensor_reading;
};
struct MainT {
    unsigned int m1_dir, m2_dir, m1_pow, m2_pow;
};
struct RemoteR {
    unsigned short rc1, rc2, rc3, rc4, rc5, rc6;
};
struct RemoteT {};

double rc1, rc2, rc3, rc4, rc5, rc6;

static inline constexpr double rc_range(const unsigned short val, const unsigned short min_val, const unsigned short mid_val,
                                        const unsigned short max_val) {
    if (val > mid_val) {
        if (val > max_val) return 0;
        return static_cast<double>(val - mid_val) / (mid_val - min_val);
    } else {
        if (val < min_val) return 0;
        return static_cast<double>(val - mid_val) / (max_val - mid_val);
    }
}

static inline void print_data() {
    debug::log::info("\033[s");
    debug::log::info("┌────────────┬─────────────┬─────────┐\n");
    debug::log::info("|    data    |    value    |  units  |\n");
    debug::log::info("├────────────┼─────────────┼─────────┤\n");
    debug::log::info("| rc1        | %*d |         |\n", 11, rc1);
    debug::log::info("| rc2        | %*d |         |\n", 11, rc2);
    debug::log::info("| rc3        | %*d |         |\n", 11, rc3);
    debug::log::info("| rc4        | %*d |         |\n", 11, rc4);
    debug::log::info("| rc5        | %*d |         |\n", 11, rc5);
    debug::log::info("| rc6        | %*d |         |\n", 11, rc6);
    debug::log::info("└────────────┴─────────────┴─────────┘\n");
    debug::log::info("\033[u");
}

int main() {
    DEBUG_PROFILE_SESSION(Setup, "setup.json");

    auto window = coel::create_window(800, 800, "test");
    auto shader = coel::create_shader(vert_src, frag_src);
    // auto main_ard = arduino::connect(sizeof(MainR), sizeof(MainT), 115200, 0xA5);
    // auto remote_ard = arduino::connect(sizeof(RemoteR), sizeof(RemoteT), 115200, 0xBD);
    // auto cam = camera::open(1280, 720);

    float vertices[] = {-1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0};
    auto vbo = coel::create_vbo(vertices, sizeof(vertices), {{coel::Type::f32, 2}});
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};
    auto ibo = coel::create_ibo(indices, sizeof(indices));
    auto vao = coel::create_vao();
    vao->add_vbo(vbo);

    if (window /* && main_ard.is_valid && remote_ard.is_valid && cam.is_open */) {
        DEBUG_PROFILE_SESSION(Loop, "loop.json");
        auto now = debug::timer::now(), start = now;

        while (!window->should_close()) {            
            // camera::update(cam);

            // arduino::update(main_ard);
            // arduino::update(remote_ard);

            // RemoteR &remote_r = *reinterpret_cast<RemoteR *>(remote_ard.rdata_handle);
            // RemoteT &remote_t = *reinterpret_cast<RemoteT *>(remote_ard.tdata_handle);

            // rc1 = rc_range(remote_r.rc1, 991, 1503, 2015); // 991 - 1503 - 2015 l-
            // rc2 = rc_range(remote_r.rc2, 984, 1506, 2008); // 984 - 1506 - 2008 r|
            // rc3 = rc_range(remote_r.rc3, 978, 1451, 2001); // 978 - 1451 - 2001 l|
            // rc4 = rc_range(remote_r.rc4, 991, 1503, 2014); // 991 - 1503 - 2014 r-
            // rc5 = rc_range(remote_r.rc5, 985, 1503, 2009); // 985 - 1503 - 2009
            // rc6 = rc_range(remote_r.rc6, 978, 1506, 2003); // 978 - 1506 - 2003

            if (now - start > arduino::TICK_INTERVAL) {
                // print_data();
                start += arduino::TICK_INTERVAL;
            }

            coel::Renderer::clear();
            vao->draw(ibo);
            window->update();
            now = debug::timer::now();
        }
    }

    DEBUG_PROFILE_SESSION(Shutdown, "shutdown.json");

    window->close();
    // arduino::disconnect(main_ard);
    // arduino::disconnect(remote_ard);
    // camera::close(cam);
}
