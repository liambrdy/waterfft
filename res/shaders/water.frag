#version 460 core

layout (location = 0) out vec4 albedo_out;
layout (location = 1) out vec4 worldPosition_out;
layout (location = 2) out vec4 normal_out;
layout (location = 3) out vec4 specular_emmision_diffuse_ssao_bloom_out;
layout (location = 4) out vec4 lightScattering_out;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inTangent;

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

uniform sampler2D normalMap;
uniform sampler2D waterReflection;
uniform sampler2D waterRefraction;
uniform sampler2D dudvMap;

uniform float motion;
uniform float distortion;
uniform vec2 wind;

const float Eta = 0.15;
//const float Eta = 0.00001;
const float zfar = 10000;
const float znear = 0.1;

float fresnelApproximated(vec3 normal, vec3 vertexToEye) {
    vec3 halfDirection = normalize(normal + vertexToEye);

    float cosine = dot(halfDirection, vertexToEye);

    float fresnel = Eta + (1.0 - Eta) * pow(max(0.0, 1.0 - dot(vertexToEye, normal)), fresnelFactor);

    return clamp(pow(fresnel, 1.0), 0.0, 1.0);
}

void main() {
    vec3 vertexToEye = normalize(eye - inPosition);
    float dist = length(eye - inPosition);

    vec2 waveMotion = wind * vec2(motion);

    vec3 normal = texture(normalMap, inUV + waveMotion).rgb;
    normal = normalize(normal);

    float fresnel = fresnelApproximated(normal.xyz, vertexToEye);

    if (dist < highDetailRange-50.0){
        float attenuation = clamp(-dist/(highDetailRange-50) + 1,0.0,1.0);
        vec3 bitangent = normalize(cross(inTangent, normal));
        mat3 TBN = mat3(inTangent,bitangent,normal);
        vec3 bumpNormal = texture(normalMap, inUV * capillarDownsampling + waveMotion).rgb;
        bumpNormal.z *= capillarStrength;
        bumpNormal.xy *= attenuation;
        bumpNormal = normalize(bumpNormal);
        normal = normalize(TBN * bumpNormal);
    }

    vec3 dudvCoord = normalize((2 * texture(dudvMap, inUV * dudvDownsampling + distortion).rbg) - 1);
    vec2 projCoord = vec2(gl_FragCoord.x/windowWidth, gl_FragCoord.y/windowHeight);

    vec2 reflectionCoords = projCoord.xy + dudvCoord.rb * kReflection;
    reflectionCoords = clamp(reflectionCoords, kReflection, 1 - kReflection);
    vec3 reflection = mix(texture(waterReflection, reflectionCoords).rgb, waterColor, reflectionBlendFactor);
    reflection *= fresnel;

    vec2 refractionCoords = projCoord.xy + dudvCoord.rb * kRefraction;
    refractionCoords = clamp(refractionCoords, kRefraction, 1 - kRefraction);

    vec3 refraction = texture(waterRefraction, refractionCoords).rgb;
    refraction *= 1 - fresnel;

    vec3 fragColor = reflection + refraction;
    //fragColor = dudvCoord;

    albedo_out = vec4(fragColor, 1.0);
    worldPosition_out = vec4(inPosition, 1.0);
    normal_out = vec4(normal, 1);
    specular_emmision_diffuse_ssao_bloom_out = vec4(200, 2.0, 0, 1);
    lightScattering_out = vec4(0, 0, 0, 1);
}
