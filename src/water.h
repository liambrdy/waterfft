#ifndef WATER_H_
#define WATER_H_

#include "types.h"
#include "texture.h"
#include "shader.h"
#include "framebuffer.h"
#include "mesh.h"
#include "camera.h"

#include "fft.h"
#include "normalRenderer.h"

#include <vector>
#include <chrono>
#include <functional>

struct WaterConfig {
    u32 N = 256;
    u32 L = 1000;
    f32 amplitude = 2.0f;
    glm::vec2 windDirection = glm::vec2(1.0f, 1.0f);
    f32 windSpeed = 80.0f;
    f32 alignment = 0.0f;
    f32 capillarWaveSupression = 0.1f;
    f32 displacementScale = 6.0f;
    f32 choppiness = 14.0f;
    u32 tessellationFactor = 20000;
    f32 tessellationShift = 0.0f;
    f32 tessellationSlope = 1.8f;
    u32 highDetailRange = 500;
    u32 uvScale = 50;
    f32 kReflection = 0.008f;
    f32 kRefraction = 0.008f;
    f32 distortion_delta = 0.0001f;
    f32 fresnelFactor = 5.0f;
    f32 waveMotion_delta = 0.00002f;
    f32 normalStrength = 0.5;
    f32 t_delta = 0.004f;
    b8 choppy = true;
    glm::vec3 baseColor = glm::vec3(0.064f, 0.085f, 0.12f);
    //glm::vec3 baseColor = glm::vec3(0.0f);
    f32 reflectionBlendFactor = 0.0f;
    f32 capillarStrength = 1.0f;
    f32 capillarDownsampling = 8.0f;
    f32 dudvDownsampling = 1.0f;
};

struct Water {
    Shader shader;

    FFT fft;
    NormalRenderer normalRenderer;

    Texture reflectionTexture, refractionTexture;
    Framebuffer reflectionFbo, refractionFbo;

    Texture dudvTexture;

    Mesh plane;

    u32 ssbo;

    long begin;
    f32 motion, distortion;

    glm::mat4 transform;

    WaterConfig waterConfig;
};

void WaterCreate(Water *water, u32 patches);

void WaterUpdate(Water *water);
void WaterRender(Water *water, Camera *camera, std::function<void()> renderFunc);

#endif // WATER_H_
