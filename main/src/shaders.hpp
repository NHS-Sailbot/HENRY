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
} // namespace shaders
