#version 460 core

layout (triangles) in;

layout (triangle_strip, max_vertices = 3) out;

layout (location = 0) in vec2 inUV[];

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outTangent;

layout (std140) uniform Camera {
    vec3 eye;
    mat4 view;
    mat4 viewProj;
    vec4 frustumPlanes[6];
};

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

uniform sampler2D dY;
uniform sampler2D dX;
uniform sampler2D dZ;

uniform float motion;
uniform vec2 wind;

const int displacementRange = 1000;

vec3 calcTangent() {
    vec3 v0 = gl_in[0].gl_Position.xyz;
    vec3 v1 = gl_in[1].gl_Position.xyz;
    vec3 v2 = gl_in[2].gl_Position.xyz;

    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    float dU1 = inUV[1].x - inUV[0].x;
    float dV1 = inUV[1].y - inUV[0].y;
    float dU2 = inUV[2].x - inUV[0].x;
    float dV2 = inUV[2].y - inUV[0].y;

    float f = 1.0 / (dU1 * dV2 - dU2 * dV1);

    vec3 t;

    t.x = f * (dV2 * e1.x - dV1 * e2.x);
    t.y = f * (dV2 * e1.y - dV1 * e2.y);
    t.z = f * (dV2 * e1.z - dV1 * e2.z);

    t = normalize(t);
    return t;
}

void main() {
    vec4[] positions = { gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position };
    vec3 tangent = vec3(0);

    vec3 eyePosition = eye;
    float dist = (distance(gl_in[0].gl_Position.xyz, eyePosition) + distance(gl_in[1].gl_Position.xyz, eyePosition) + distance(gl_in[0].gl_Position.xyz, eyePosition))/3;
    if (dist < displacementRange+100) {
        if (dist < highDetailRange) {
            tangent = calcTangent();
        }

        for (int i = 0; i < gl_in.length(); i++) {
            float dy = texture(dY, inUV[i]+(wind*motion)).r * max(0,(- distance(gl_in[i].gl_Position.xyz, eyePosition)/displacementRange + 1)) * displacementScale;
            float dx = texture(dX, inUV[i]+(wind*motion)).r * max(0,(- distance(gl_in[i].gl_Position.xyz, eyePosition)/displacementRange + 1)) * choppiness;
            float dz = texture(dZ, inUV[i]+(wind*motion)).r * max(0,(- distance(gl_in[i].gl_Position.xyz, eyePosition)/displacementRange + 1)) * choppiness;

            positions[i].y += dy;
            positions[i].x -= dx;
            positions[i].z -= dz;
        }
    }

    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = viewProj * positions[i];
        gl_ClipDistance[0] = dot(gl_Position, frustumPlanes[0]);
        gl_ClipDistance[1] = dot(gl_Position, frustumPlanes[1]);
        gl_ClipDistance[2] = dot(gl_Position, frustumPlanes[2]);
        gl_ClipDistance[3] = dot(gl_Position, frustumPlanes[3]);
        gl_ClipDistance[4] = dot(gl_Position, frustumPlanes[4]);
        gl_ClipDistance[5] = dot(gl_Position, frustumPlanes[5]);
        outUV = inUV[i];
        outPos = positions[i].xyz;
        //outPos = gl_Position.xyz;
        outTangent = tangent;
        EmitVertex();
    }
}
