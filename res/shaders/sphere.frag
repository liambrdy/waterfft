#version 460 core

layout (location = 0) out vec4 albedo_out;
layout (location = 1) out vec4 worldPosition_out;
layout (location = 2) out vec4 normal_out;
layout (location = 3) out vec4 specular_emmision_diffuse_ssao_bloom_out;
layout (location = 4) out vec4 lightScattering_out;

in vec4 fragPosition;
in vec3 fragNormal;
in vec2 fragUV;

void main() {
    albedo_out = vec4(fragNormal, 1.0);
    worldPosition_out = fragPosition;
    normal_out = vec4(fragNormal, 1.0);
    specular_emmision_diffuse_ssao_bloom_out = vec4(1, 0, 0, 0);
    lightScattering_out = vec4(0, 0, 0, 0);
}
