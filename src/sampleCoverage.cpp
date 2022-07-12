#include "sampleCoverage.h"

#include "renderer.h"

void SampleCoverageCreate(SampleCoverage *coverage, u32 width, u32 height) {
    TextureCreate(&coverage->sampleCoverageMask, width, height, R8, Nearest, ClampToEdge);
    TextureCreate(&coverage->lightScatteringMaskSingleSample, width, height, RGBA16Float, Nearest, ClampToEdge);
    TextureCreate(&coverage->specularEmissionBloomMaskSingleSample, width, height, RGBA16Float, Nearest, ClampToEdge);

    std::vector<float> red;
    for (i32 i = 0; i < 512; i++) {
        for (i32 j = 0; j < 512; j++) {
            red.push_back(1.0f);
            red.push_back(1.0f);
            red.push_back(0.0f);
            red.push_back(1.0f);
        }
    }

    TextureCreate(&coverage->tmp, 512, 512, RGBA32Float, Bilinear, Repeat);
    TextureBind(&coverage->tmp);
    glTexSubImage2D(coverage->tmp.target, 0, 0, 0, 512, 512, GL_RGBA32F, GL_FLOAT, red.data());

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/sampleCoverage.comp"};
    info.shaderBits.set(ComputeShader);
    ShaderCreate(&coverage->shader, &info);
}

void SampleCoverageRender(SampleCoverage *coverage, Texture *worldPositionTexture, Texture *lightScatteringMask, Texture *specularEmissionBloomMask) {
    ShaderBind(&coverage->shader);

    glBindImageTexture(0, coverage->sampleCoverageMask.id, 0, false, 0, GL_WRITE_ONLY, GL_R8);
    glBindImageTexture(1, coverage->tmp.id, 0, false, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(2, coverage->lightScatteringMaskSingleSample.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
    glBindImageTexture(3, lightScatteringMask->id, 0, false, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(4, coverage->specularEmissionBloomMaskSingleSample.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
    glBindImageTexture(5, specularEmissionBloomMask->id, 0, false, 0, GL_READ_ONLY, GL_RGBA16F);

    ShaderSetInt(&coverage->shader, "multisamples", renderer.samples);

    glDispatchCompute(renderer.windowWidth / 16, renderer.windowHeight / 16, 1);
}
