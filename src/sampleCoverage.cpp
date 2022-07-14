#include "sampleCoverage.h"

#include "renderer.h"

void SampleCoverageCreate(SampleCoverage *coverage, u32 width, u32 height) {
    TextureCreate(&coverage->sampleCoverageMask, width, height, R8, Nearest, ClampToEdge);
    TextureCreate(&coverage->lightScatteringMaskSingleSample, width, height, RGBA16Float, Nearest, ClampToEdge);
    TextureCreate(&coverage->specularEmissionBloomMaskSingleSample, width, height, RGBA16Float, Nearest, ClampToEdge);

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/sampleCoverage.comp"};
    info.shaderBits.set(ComputeShader);
    ShaderCreate(&coverage->shader, &info);
}

void SampleCoverageRender(SampleCoverage *coverage, Texture *worldPositionTexture, Texture *lightScatteringMask, Texture *specularEmissionBloomMask) {
    ShaderBind(&coverage->shader);

    // glBindImageTexture(0, coverage->sampleCoverageMask.id, 0, false, 0, GL_WRITE_ONLY, GL_R8);
    // glBindImageTexture(1, worldPositionTexture->id, 0, false, 0, GL_READ_ONLY, GL_RGBA32F);
    // glBindImageTexture(2, coverage->lightScatteringMaskSingleSample.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
    // glBindImageTexture(3, lightScatteringMask->id, 0, false, 0, GL_READ_ONLY, GL_RGBA16F);
    // glBindImageTexture(4, coverage->specularEmissionBloomMaskSingleSample.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
    // glBindImageTexture(5, specularEmissionBloomMask->id, 0, false, 0, GL_READ_ONLY, GL_RGBA16F);

    glBindImageTexture(0, coverage->sampleCoverageMask.id, 0, false, 0, GL_READ_WRITE, GL_R8);
    glBindImageTexture(1, worldPositionTexture->id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(2, coverage->lightScatteringMaskSingleSample.id, 0, false, 0, GL_READ_WRITE, GL_RGBA16F);
    glBindImageTexture(3, lightScatteringMask->id, 0, false, 0, GL_READ_WRITE, GL_RGBA16F);
    glBindImageTexture(4, coverage->specularEmissionBloomMaskSingleSample.id, 0, false, 0, GL_READ_WRITE, GL_RGBA16F);
    glBindImageTexture(5, specularEmissionBloomMask->id, 0, false, 0, GL_READ_WRITE, GL_RGBA16F);

    ShaderSetInt(&coverage->shader, "multisamples", renderer.samples);

    glDispatchCompute(renderer.windowWidth / 16, renderer.windowHeight / 16, 1);
}
