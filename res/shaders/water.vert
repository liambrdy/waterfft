#version 460 core

layout (location = 0) in vec2 a_Position;

layout (location = 0) out vec2 outUV;

layout (std430, binding = 1) buffer ssbo {
    mat4 worldMatrix;
    int uvScale;
    int tessFactor;
    float tessSlope;
    float tessShift;
    float displacementScale;
    int highDetailRange;
    float choppiness;
    float kReflection;
    float kRefraction;
    int windowWidth;
    int windowHeight;
    float reflectionBlendFactor;
    vec3 waterColor;
    float fresnelFactor;
    float capillarStrength;
    float capillarDownsampling;
    float dudvDownsampling;
};

uniform mat4 world;

void main() {
    gl_Position = worldMatrix * vec4(a_Position.x, 0.0, a_Position.y, 1.0);
    outUV = a_Position;
}
