#version 460 core

layout (location = 0) out vec4 albedo_out;
layout (location = 1) out vec4 worldPosition_out;
layout (location = 2) out vec4 normal_out;
layout (location = 3) out vec4 specular_emmision_diffuse_ssao_bloom_out;
layout (location = 4) out vec4 lightScattering_out;

in vec3 outUV;

uniform samplerCube skybox;
uniform mat4 viewProj;

uniform int width;
uniform int height;

const vec3 sunBaseColor = vec3(1.0,0.79,0.43);
const float r_Sun = 0.25;
const vec3 v_SunWorld = vec3(-0.6527264, -0.42568117, -0.62669265);

void main() {
    float red = -1 * (outUV.y - 0.75);
    float green = -1.2 * (outUV.y - 0.8);
    float blue = -0.9 * (outUV.y - 1.1);

    vec3 outColor = vec3(red, green, blue);
    vec3 outLightScattering = vec3(0);

    vec4 ndc = vec4(
        (gl_FragCoord.x / width - 0.5) * 2.0,
        (gl_FragCoord.y / height - 0.5) * 2.0,
        (gl_FragCoord.z - 0.5) * 2.0,
        1.0);

    vec4 clip = inverse(viewProj) * ndc;
    vec3 v_World = (clip / clip.w).xyz;

    float sunRadius = length(normalize(v_World)- normalize(v_SunWorld));

    if(sunRadius < r_Sun)
    {
        sunRadius /= r_Sun;
        float smoothRadius = smoothstep(0,1,0.1f/sunRadius-0.1f);
        outColor = mix(outColor, sunBaseColor * 4, smoothRadius);

        smoothRadius = smoothstep(0,1,0.18f/sunRadius-0.2f);
        outLightScattering = mix(vec3(0), sunBaseColor, smoothRadius);
    }

    //albedo_out = texture(skybox, outUV);
    albedo_out = vec4(outColor, 1.0);
    worldPosition_out = vec4(0, 0, 0, 0);
    normal_out = vec4(0, 0, 0, 0);
    specular_emmision_diffuse_ssao_bloom_out = vec4(0, 0, 0, 0);
    lightScattering_out = vec4(outLightScattering, 0);
}
