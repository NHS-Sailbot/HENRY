#include "../engine/include/coel/coel.hpp"
#include "../gpu_cuda/cuda.hpp"
#include <HENRY/camera.hpp>
#include <fstream>

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
layout (binding = 0) uniform sampler2D tex;

out vec4 color;

void main() {
    color = texture(tex, v_tex);
    // color = vec4(0.1, 0.1, 0.13, 1);
    // color = vec4(v_tex, 0, 1);
}
)";

coel::Ref<coel::Texture> texture;

void on_camera_update(const HENRY::Camera &c) {
    if (!(c.data == nullptr) && c.data_size > 0) {
        cuda::perform_kernel(c.pixels);
        texture->set_data(c.pixels);
    }
}

using namespace HENRY;

int main() {
    Camera cam(1280, 720);
    auto window = coel::create_window(cam.width, cam.height, "test");
    cuda::init();

    float vertices[] = {
        -1.0, -1.0, //
        0,    1,    //
        -1.0, 1.0,  //
        0,    0,    //
        1.0,  -1.0, //
        1,    1,    //
        1.0,  1.0,  //
        1,    0,    //
    };
    auto vbo = coel::create_vbo(vertices, sizeof(vertices), {{coel::Type::f32, 2}, {coel::Type::f32, 2}});

    unsigned short indices[] = {0, 1, 2, 1, 3, 2};
    auto ibo = coel::create_ibo(indices, sizeof(indices));

    auto vao = coel::create_vao();
    vao->add_vbo(vbo);

    auto shader = coel::create_shader(vert_src, frag_src);
    texture = coel::create_texture(1280, 720, nullptr);
    texture->bind(0);
    cam.on_update = on_camera_update;

    if (window && cam.is_open) {
        while (!window->should_close()) {
            cam.update();
            vao->draw(ibo);
            window->update();
        }
    }

    cuda::deinit();
}
