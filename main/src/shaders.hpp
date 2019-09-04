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
} // namespace shaders