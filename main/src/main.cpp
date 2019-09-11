#include <sailbot.hpp>
#include <stdio.h>

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
    // printf("%f, %f | %f | ", rdata.gps_lat, rdata.gps_lat, rdata.wind_direction);
    printf("fps: %f\n", 1.0 / sailbot::system::TICK_DURATION * frames_per_tick);
    frames_per_tick = 0;
}

static constexpr char *vert_src = R"(
    #version 330 core
    layout(location = 0) in vec2 pos;
    layout(location = 1) in vec2 tex;
    layout(location = 2) in vec4 col;
    layout(location = 3) in float tid;
    
    out vec2 v_tex;
    out float v_tid;
    
    void main() {
        v_tex = tex;
        v_tid = tid;
        gl_Position = vec4(pos, 0, 1);
    }
)";

static constexpr char *frag_src = R"(
    #version 330 core
    in vec2 v_tex;
    in float v_tid;
    
    uniform sampler2D tex;
    uniform vec3 key_color;
    
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
        vec3 key = key_color / 256;
        switch (int(v_tid + 0.5)) {
            case 0: 
                color = texture(tex, v_tex);
                if (key.x - 0.8 < color.x && color.x < key.x + 0.8 && 
                    key.y - 0.01 < color.y && color.y < key.y + 0.01 && 
                    key.z - 0.01 < color.z && color.z < key.z + 0.01)
                    {
                        if (int(gl_FragCoord.x) % 10 < 5) {
                            if (int(gl_FragCoord.y) % 10 < 5) color = vec4(1, 0, 1, 1);
                            else color = vec4(0.8, 0, 0.8, 1);
                        }
                        else {
                            if (int(gl_FragCoord.y) % 10 < 5) color = vec4(0.8, 0, 0.8, 1);
                            else color = vec4(1, 0, 1, 1);
                        }
                    }
                else {
                    color = yuv422_rgb(color);
                }
                break;
            case 1: color = yuv422_rgb(vec4(key, 1)); break;
        }
        
    }
)";

static unsigned char image_data[sailbot::camera::width * sailbot::camera::height * 3];
static math::Vec3 key_color = {0, 0, 0};

struct MainWindow : coel::Window {
    math::Vec2 mouse = {0, 0}, size, selection_cursor = {0, 0};
    MainWindow(const unsigned int width, const unsigned int height, const char *const title)
        : coel::Window(width, height, title), size{float(width), float(height)} {}
    void mouse_move(const coel::MouseMove &e) override { mouse = {float(e.x), float(e.y)}; }
    void mouse_press(const coel::MousePress &e) override {
        const unsigned int index = int(mouse.x / size.x * sailbot::camera::width) * 3 +
                                   3 * sailbot::camera::width * int(mouse.y / size.y * sailbot::camera::height);
        key_color = {float(image_data[index]), float(image_data[index + 1]), float(image_data[index + 2])};
        printf("%f, %f, %f\n", key_color.x, key_color.y, key_color.z);
    }
    void window_resize(const coel::WindowResize &e) override { size = {float(e.width), float(e.height)}; }
};

int main() {
    sailbot::callbacks::set::on_data_read(tick);
    sailbot::system::init("/dev/ttyACM0", 9600);
    sailbot::camera::open();

    MainWindow window(sailbot::camera::width, sailbot::camera::height, "test");
    coel::Texture texture(sailbot::camera::width, sailbot::camera::height, coel::ColorSpace::RGB);
    coel::Shader shader(vert_src, frag_src);
    coel::renderer::batch2d::init();

    double last_time = window.get_time();

    while (!window.should_close()) {
        sailbot::system::update(&tdata, sizeof(tdata), &rdata, sizeof(rdata));

        unsigned char *buffer_start = sailbot::camera::read();
        if (buffer_start == nullptr) buffer_start = image_data;
        unsigned char *cam_y = buffer_start, *cam_u = buffer_start + 1, *cam_v = buffer_start + 3;
        for (unsigned int i = 0; i < sailbot::camera::width * sailbot::camera::height * 3; i += 3) {
            image_data[i + 0] = *cam_y;
            image_data[i + 1] = *cam_u;
            image_data[i + 2] = *cam_v;
            if (i % 2) { cam_u += 4, cam_v += 4; }
            cam_y += 2;
        }

        texture.update(image_data);

        coel::renderer::clear();
        coel::renderer::batch2d::submit_rect(-1, 1, 2, -2, 0.f);
        coel::renderer::batch2d::submit_rect(1, -1, -0.1, 0.1, 1.f);
        coel::renderer::batch2d::flush();

        shader.send_float3("key_color", reinterpret_cast<float *>(&key_color));

        window.update();

        ++frames_per_tick;
    }

    sailbot::camera::close();
}
