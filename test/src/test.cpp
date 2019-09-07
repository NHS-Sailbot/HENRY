#include <sailbot.hpp>

#include <iostream>
#include <memory.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include <linux/v4l2-subdev.h>

static constexpr unsigned int capture_width = 1280, capture_height = 720;
struct Vertex {
    float x, y, u, v;
};

static constexpr Vertex vertices[]{
    {-1, -1, 0, 1},
    {-1, 1, 0, 0},
    {1, -1, 1, 1},
    {1, 1, 1, 0},
};

static constexpr unsigned short indices[] = {0, 1, 2, 1, 3, 2};

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
        col -= vec4(0.0625, 0.5, 0.5, 0);
        vec4 result = vec4(
            1.1640625 * col.x + 1.59765625 * col.z + 0.5,
            0,0,1
        );
        return result;
    }
    
    void main() {
        color = vec4(v_tex, 0, 1);
        color = yuv422_rgb(texture(tex, v_tex));
    }
)";

// const int c = *cam_y - 16, d = *cam_u - 128, e = *cam_v - 128;
// image_r = clamp((298 * c + 409 * e + 128) >> 8);
// image_g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
// image_b = clamp((298 * c + 516 * d + 128) >> 8);

int main() {
    sailbot::camera::open();
    sailbot::camera::get_capability();
    sailbot::camera::set_format();
    sailbot::camera::set_framerate();
    sailbot::camera::create_buffers();

    coel::Window window(capture_width, capture_height, "test");
    coel::renderer::batch2d::init();
    coel::Shader shader(vert_src, frag_src);
    coel::Texture texture(capture_width, capture_height, coel::ColorSpace::RGB);

    double last_time = window.get_time();
    while (!window.should_close()) {
        const double time = window.get_time();
        const double elapsed = time - last_time;
        last_time = time;
        std::cout << "elapsed time is " << elapsed << '\n';
        unsigned char image_data[capture_width * capture_height * 3];
        unsigned char *buffer_start = sailbot::camera::read();
        if (buffer_start == nullptr) {
            printf(".");
            buffer_start = image_data;
        }
        unsigned char *cam_y = buffer_start, *cam_u = buffer_start + 1, *cam_v = buffer_start + 3;
        for (unsigned int y = 0; y < capture_height; ++y) {
            for (unsigned int x = 0; x < capture_width; ++x) {
                const unsigned int index = x * 3 + y * capture_width * 3;
                image_data[index] = *cam_y;
                image_data[index + 1] = *cam_u;
                image_data[index + 2] = *cam_v;
                if (index % 2) { cam_u += 4, cam_v += 4; }
                cam_y += 2;
                // const unsigned int index = x * 3 + y * capture_width * 3;
                // unsigned char &image_r = image_data[index];
                // unsigned char &image_g = image_data[index + 1];
                // unsigned char &image_b = image_data[index + 2];
                // const int c = *cam_y - 16, d = *cam_u - 128, e = *cam_v - 128;
                // image_r = clamp((298 * c + 409 * e + 128) >> 8);
                // image_g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8);
                // image_b = clamp((298 * c + 516 * d + 128) >> 8);
                // if (index % 2) { cam_u += 4, cam_v += 4; }
                // cam_y += 2;
            }
        }

        //
        //
        //
        //
        //

        texture.update(image_data);

        coel::renderer::clear();
        coel::renderer::batch2d::submit_rect(-0.9, 0.9, 1.8, -1.8);
        coel::renderer::batch2d::flush();

        window.update();
    }

    //
    //
    //
    //
    //
}
