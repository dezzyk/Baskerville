R"(
#version 430 core

layout (location = 0) in vec4 v_color;
layout (location = 1) in vec3 v_tex_coords;
layout (location = 2) in vec2 v_pos;

out vec3 f_tex_coords;

layout (std140) uniform matrices {
    mat4 proj;
    mat4 model;
};

void main() {
   gl_Position = proj * model * vec4(v_pos, 0.0, 1.0);
   f_tex_coords = v_tex_coords;
};
)"