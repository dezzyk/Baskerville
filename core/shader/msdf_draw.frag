R"(
#version 430 core

in vec3 f_tex_coords;
in vec4 f_color;

out vec4 draw_color;

layout (location = 2) uniform sampler2DArray u_msdf;
layout (location = 3) uniform float u_pixel_range;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main(){
    vec3 flipped_tex_coords = vec3(f_tex_coords.x, 1.0 - f_tex_coords.y, f_tex_coords.z);
    vec2 pos = flipped_tex_coords.xy;
    vec3 texture_sample = texture(u_msdf, flipped_tex_coords).rgb;
    float sig_dist = median(texture_sample.r, texture_sample.g, texture_sample.b);
    float w = fwidth(sig_dist);
    float opacity = smoothstep(0.5 - w, 0.5 + w, sig_dist);
    draw_color = vec4(f_color.rgb, opacity);
}
)"