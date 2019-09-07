#pragma once

namespace shaders {
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
            298.0/256 * col.x + 409.0/256 * col.z + 0.5,
            298.0/256 * col.x + 100.0/256 * col.y - 208.0/256 * col.z + 0.5,
            298.0/256 * col.x + 516.0/256 * col.y + 0.5,
            1
        );
        return result;
    }
    
    void main() {
        color = yuv422_rgb(texture(tex, v_tex)) / 2;
    }
    )";
} // namespace shaders
