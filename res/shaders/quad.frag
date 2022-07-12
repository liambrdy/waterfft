#version 460 core

layout (location = 0) out vec4 color;

in vec2 outTexCoord;

uniform sampler2D tex;

void main() {
    vec4 sampled = texture(tex, outTexCoord);

    if (sampled.a == 0.0)
        discard;

    color = sampled;
}
