#define DEBUG_ENABLE_TIMING
#include <debug/debug.hpp>

#include "../engine/include/coel/coel.hpp"

#include <HENRY/arduino.hpp>
#include <HENRY/camera.hpp>

struct MainR {
    unsigned int wind_sensor_reading;
    int motor_reading;
};
struct MainT {
    int motor_desired;
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

static bool key_left = false, key_right = false;
static constexpr int MOTOR_REV = 6110;
static int mot_desired = MOTOR_REV;

static inline void main_update(HENRY::Arduino &ard) {
    MainR &main_r = *reinterpret_cast<MainR *>(ard.rdata_handle);
    MainT &main_t = *reinterpret_cast<MainT *>(ard.tdata_handle);
    // for (unsigned int i = 0; i < ard.tsize; ++i)
    //     debug::log::info("%2x ", ard._tdata_buffer[i]);
    // debug::log::info("\n");
    // for (unsigned int i = 0; i < ard.rsize; ++i)
    //     debug::log::info("%2x ", ard._rdata_buffer[i]);
    // debug::log::info("%d, %d\n", mot_desired, main_r.motor_reading);
}

static void key_press(const coel::KeyPress &e) {
    if (e.key == coel::Up)
        mot_desired += MOTOR_REV;
    else if (e.key == coel::Down)
        mot_desired -= MOTOR_REV;
    else if (e.key == coel::Left)
        key_left = true;
    else if (e.key == coel::Right)
        key_right = true;
}

static void key_release(const coel::KeyRelease &e) {
    if (e.key == coel::Left)
        key_left = false;
    else if (e.key == coel::Right)
        key_right = false;
}

//-46 -> 61110 = 61156 = 10 * 6116

static constexpr const char *const vert_src = R"(
#version 420
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 v_tex;

void main() {
    v_tex = tex;
    gl_Position = vec4(pos, 0, 1);
}
)",
                                   *const frag_src = R"(
#version 420

in vec2 v_tex;
layout (binding = 1) uniform sampler2D tex;

out vec4 color;

void main() {
    color = texture(tex, v_tex);
    // color = vec4(0.1, 0.1, 0.13, 1);
    // color = vec4(v_tex, 0, 1);
}
)";

coel::Ref<coel::Texture> texture;

void on_camera_update(const HENRY::Camera &c) {
    if (!(c.data == nullptr) && c.data_size > 0) texture->set_data(c.pixels);
}

int main() {
    DEBUG_PROFILE_SESSION(Setup, "out/setup.json");

    // HENRY::Camera cam(1280, 720);

    auto window = coel::create_window(200, 200, "test");

    // float vertices[] = {
    //     -1.0, -1.0, //
    //     0,    1,    //
    //     -1.0, 1.0,  //
    //     0,    0,    //
    //     1.0,  -1.0, //
    //     1,    1,    //
    //     1.0,  1.0,  //
    //     1,    0,    //
    // };
    // auto vbo = coel::create_vbo(vertices, sizeof(vertices), {{coel::Type::f32, 2}, {coel::Type::f32, 2}});

    // unsigned short indices[] = {0, 1, 2, 1, 3, 2};
    // auto ibo = coel::create_ibo(indices, sizeof(indices));

    // auto vao = coel::create_vao();
    // vao->add_vbo(vbo);

    // auto shader = coel::create_shader(vert_src, frag_src);
    // texture = coel::create_texture(1280, 720, nullptr);
    // texture->bind(1);
    // cam.on_update = on_camera_update;

    window->on_key_press(key_press);
    window->on_key_release(key_release);
    HENRY::Arduino main_ard(sizeof(MainR), sizeof(MainT), 115200, 0xA5);
    // HENRY::Arduino remote_ard(sizeof(RemoteR), sizeof(RemoteT), 115200, 0xBD);

    if (/*window && */ main_ard.is_valid /* && remote_ard.is_valid &&  cam.is_open*/) {
        DEBUG_PROFILE_SESSION(Loop, "out/loop.json");
        auto now = debug::timer::now(), start = now;
        main_ard.on_update = main_update;

        MainR &main_r = *reinterpret_cast<MainR *>(main_ard.rdata_handle);
        MainT &main_t = *reinterpret_cast<MainT *>(main_ard.tdata_handle);

        while (!window->should_close()) { //
            // cam.update();

            // vao->draw(ibo);

            main_ard.update();
            // remote_ard.update();

            // RemoteR &remote_r = *reinterpret_cast<RemoteR *>(remote_ard.rdata_handle);
            // RemoteT &remote_t = *reinterpret_cast<RemoteT *>(remote_ard.tdata_handle);

            // rc1 = rc_range(remote_r.rc1, 991, 1503, 2015); // 991 - 1503 - 2015 l-
            // rc2 = rc_range(remote_r.rc2, 984, 1506, 2008); // 984 - 1506 - 2008 r|
            // rc3 = rc_range(remote_r.rc3, 978, 1451, 2001); // 978 - 1451 - 2001 l|
            // rc4 = rc_range(remote_r.rc4, 991, 1503, 2014); // 991 - 1503 - 2014 r-
            // rc5 = rc_range(remote_r.rc5, 985, 1503, 2009); // 985 - 1503 - 2009
            // rc6 = rc_range(remote_r.rc6, 978, 1506, 2003); // 978 - 1506 - 2003

            // debug::log::info("\033[s");
            // debug::log::message("%d", ++motor_count);
            // debug::log::info("\033[u");

            if (key_left)
                main_t.motor_desired = main_r.motor_reading + MOTOR_REV, mot_desired = main_r.motor_reading + 250;
            else if (key_right)
                main_t.motor_desired = main_r.motor_reading - MOTOR_REV, mot_desired = main_r.motor_reading - 250;
            else
                main_t.motor_desired = mot_desired;

            window->update();
            now = debug::timer::now();
        }
    }

    DEBUG_PROFILE_SESSION(Shutdown, "out/shutdown.json");

    window->close();
}
