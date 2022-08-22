#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec3 fragPosition;

uniform mat4 modelViewProj;
uniform mat4 model;

void main() {
    gl_Position = modelViewProj * vec4(inPosition, 1.0);
    fragPosition = (model * vec4(inPosition, 1.0)).xyz;
}
