#version 460 core

layout (location = 0) in vec3 inPosition;

out vec3 outUV;

uniform mat4 viewProj;
uniform mat4 world;

void main() {
    outUV = inPosition;
    vec4 pos = viewProj * world * vec4(inPosition, 0.0);
    gl_Position = pos.xyww;
}
