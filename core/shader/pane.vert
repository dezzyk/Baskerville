R"(
#version 430 core

layout(location = 0) in vec4 v_color;
layout(location = 1) in vec3 v_tex_coords;
layout(location = 2) in vec2 v_pos;

layout (std140) uniform matrices {
    mat4 proj;
    mat4 model;
};

layout (location = 0) uniform mat4 u_proj;
layout (location = 1) uniform mat4 u_model;

out vec4 f_color;

void main() {
   gl_Position = proj * model * vec4(v_pos, 0.0, 1.0);
   f_color = v_color;
};
)"