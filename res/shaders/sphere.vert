#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec4 fragPosition;
out vec3 fragNormal;
out vec2 fragUV;

layout (std140) uniform Camera {
    vec3 eye;
    mat4 view;
    mat4 viewProj;
    vec4 frustumPlanes[6];
};

uniform mat4 model;

void main() {
    vec4 position = model * vec4(inPosition, 1.0);

    fragPosition = position;
    fragNormal = inNormal;
    fragUV = inUV;
    gl_Position = viewProj * position;
    gl_ClipDistance[0] = dot(gl_Position, frustumPlanes[0]);
    gl_ClipDistance[1] = dot(gl_Position, frustumPlanes[1]);
    gl_ClipDistance[2] = dot(gl_Position, frustumPlanes[2]);
    gl_ClipDistance[3] = dot(gl_Position, frustumPlanes[3]);
    gl_ClipDistance[4] = dot(gl_Position, frustumPlanes[4]);
    gl_ClipDistance[5] = dot(gl_Position, frustumPlanes[5]);
}
