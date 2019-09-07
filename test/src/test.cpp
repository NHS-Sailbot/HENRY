#include <sailbot.hpp>

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
    }
)";

static constexpr char *frag_src = R"(
    #version 330 core
    in vec2 v_tex;
    
    out vec4 color;
    
    uniform sampler2D tex;
    
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
        color = vec4(v_tex, 0, 1);
        color = yuv422_rgb(texture(tex, v_tex)); // texture(tex, v_tex);
    }
)";

int main() {
    sailbot::camera::open();

    coel::Window window(sailbot::camera::width, sailbot::camera::height, "test");
    coel::Texture texture(sailbot::camera::width, sailbot::camera::height, coel::ColorSpace::RGB);
    coel::Shader shader(vert_src, frag_src);
    coel::renderer::batch2d::init();

    double last_time = window.get_time();
    unsigned char image_data[sailbot::camera::width * sailbot::camera::height * 3];

    while (!window.should_close()) {
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
        coel::renderer::batch2d::submit_rect(-1, 1, 2, -2);
        coel::renderer::batch2d::flush();

        window.update();
    }

    sailbot::camera::close();
}
