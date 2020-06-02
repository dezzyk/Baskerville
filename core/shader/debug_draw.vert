R"(
#version 430 core

layout (location = 0) in vec2 v_pos;

layout (location = 0) uniform mat4 u_proj;

void main() {
   gl_Position = u_proj * vec4(v_pos, 0.0, 1.0);
};
)"