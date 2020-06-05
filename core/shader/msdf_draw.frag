R"(
#version 430 core

in vec3 f_tex_coords;
in vec4 f_color;

out vec4 draw_color;

layout (location = 2) uniform sampler2DArray u_msdf;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main(){
    vec3 flipped_tex_coords = vec3(f_tex_coords.x, 1.0 - f_tex_coords.y, f_tex_coords.z);
    vec2 pos = flipped_tex_coords.xy;
    vec3 samp = texture(u_msdf, flipped_tex_coords).rgb;
    ivec2 sz = textureSize( u_msdf, 0 ).xy;
    float dx = dFdx( flipped_tex_coords.x ) * sz.x;
    float dy = dFdy( flipped_tex_coords.y ) * sz.y;
    float toPixels = 8.0 * inversesqrt( dx * dx + dy * dy );
    float sigDist = median( samp.r, samp.g, samp.b ) - 0.5;
    float opacity = clamp( sigDist * toPixels + 0.5, 0.0, 1.0 );
    draw_color = vec4(f_color.rgb, opacity);

}
)"