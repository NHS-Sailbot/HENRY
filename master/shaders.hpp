static constexpr const char *const vert_src = R"(
#version 330

layout (location = 0) in vec2 pos;

void main() {
    gl_Position = vec4(pos, 0, 1);
}
)",
                                   *const frag_src = R"(
#version 330

out vec4 color;

void main() {
    color = vec4(0.1, 0.1, 0.13, 1);
}
)";