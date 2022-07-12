#version 460 core

layout (location = 0) out vec4 color;

in vec2 outTexCoord;

uniform sampler2DMS tex;
uniform int width;
uniform int height;
uniform int multisamples;

void main() {
    float x = outTexCoord.x * width;
    float y = outTexCoord.y * height;
    ivec2 coord = ivec2(int(x), int(y));

    vec4 rgba = vec4(0);
    for (int i = 0; i < multisamples; i++) {
        rgba += texelFetch(tex, coord, i).rgba;
    }
    rgba /= multisamples;
    // rgba = texelFetch(tex, coord, 0);
    // rgba += texelFetch(tex, coord, 1);
    // rgba /= 2;

    color = rgba;
}
