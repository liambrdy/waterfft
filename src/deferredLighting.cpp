#include "deferredLighting.h"

#include "renderer.h"

struct DirectionalLightBuffer {
    glm::vec3 direction;
    f32 intensity;
    glm::vec3 ambient;
    f32 padding00;
    glm::vec3 color;
    f32 padding01;
};

void DeferredLightingCreate(DeferredLighting *lighting) {
    TextureCreate(&lighting->deferredLightingSceneTexture, renderer.windowWidth, renderer.windowHeight, RGBA16Float, Bilinear, ClampToEdge);

    DirectionalLightBuffer buffer;
    buffer.direction = renderer.light.direction;
    buffer.intensity = renderer.light.intensity;
    buffer.ambient = renderer.light.ambient;
    buffer.color = renderer.light.color;
    buffer.padding00 = 0.0f;
    buffer.padding01 = 0.0f;

    glCreateBuffers(1, &lighting->directionalLightUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, lighting->directionalLightUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), &buffer, GL_STATIC_DRAW);

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/lighting.comp"};
    info.shaderBits.set(ComputeShader);
    ShaderCreate(&lighting->shader, &info);
}

void DeferredLightingRender(DeferredLighting *lighting, Texture *sampleCoverage, Texture *ssaoBlur, Texture *albedo, Texture *worldPosition, Texture *normal, Texture *specularEmissionDiffuseSsaoBloom) {
    glFinish();

    ShaderBind(&lighting->shader);

    CameraBind(&renderer.camera, &lighting->shader);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lighting->directionalLightUbo);
    ShaderBindUniformBlock(&lighting->shader, "DirectionalLight", 1);

    ShaderSetInt(&lighting->shader, "numSamples", renderer.samples);
    ShaderSetFloat(&lighting->shader, "sightRangeFactor", 10.0f);
    ShaderSetFloat3(&lighting->shader, "fogColor", glm::vec3(0.26f, 0.34f, 0.36f));

    glBindImageTexture(0, lighting->deferredLightingSceneTexture.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
    glBindImageTexture(1, albedo->id, 0, false, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(2, worldPosition->id, 0, false, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(3, normal->id, 0, false, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(4, specularEmissionDiffuseSsaoBloom->id, 0, false, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(5, sampleCoverage->id, 0, false, 0, GL_READ_ONLY, GL_R8);
    glBindImageTexture(6, ssaoBlur->id, 0, false, 0, GL_READ_ONLY, GL_R16F);

    glDispatchCompute(renderer.windowWidth / 2, renderer.windowHeight / 2, 1);
}
