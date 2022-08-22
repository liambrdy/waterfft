#include "ssao.h"

#include "util.h"

//#include <format>

#include "renderer.h"

void SSAOCreate(SSAO *ssao, u32 width, u32 height) {
    ssao->width = width;
    ssao->height = height;

    ssao->kernelSize = 64;

    ssao->randomX.resize(16);
    ssao->randomY.resize(16);

    for (u32 i = 0; i < 16; i++) {
        ssao->randomX[i] = rndEngine.rndF32(-1.0f, 1.0f);
        ssao->randomY[i] = rndEngine.rndF32(-1.0f, 1.0f);
    }


    ssao->kernel = GenerateRandomKernel3D(ssao->kernelSize);

    TextureCreateStorage(&ssao->noiseTexture, 4, 4, 1, RGBA16Float);
    TextureCreate(&ssao->ssaoSceneTexture, width, height, R16Float, Bilinear, ClampToEdge);
    TextureCreate(&ssao->ssaoBlurTexture, width, height, R16Float, Bilinear, ClampToEdge);

    ShaderCreateInfo info = {};
    info.shaderBits.set(ComputeShader);

    info.shaders = {"res/shaders/noise.comp"};
    ShaderCreate(&ssao->noiseTextureShader, &info);

    info.shaders = {"res/shaders/ssao.comp"};
    ShaderCreate(&ssao->ssaoShader, &info);

    info.shaders = {"res/shaders/ssao_blur.comp"};
    ShaderCreate(&ssao->blurShader, &info);

    ShaderBind(&ssao->noiseTextureShader);
    TextureBind(&ssao->noiseTexture, 0);
    glBindImageTexture(0, ssao->noiseTexture.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);

    for (u32 i = 0; i < 16; i++) {
        std::string str;
        std::sprintf(str.data(), "randomX[%d]", i);
        ShaderSetFloat(&ssao->noiseTextureShader, str, ssao->randomX[i]);
    }

    for (u32 i = 0; i < 16; i++) {
        std::string str;
        std::sprintf(str.data(), "randomY[%d]", i);
        ShaderSetFloat(&ssao->noiseTextureShader, str, ssao->randomX[i]);
    }

    glDispatchCompute(1, 1, 1);
}

void SSAORender(SSAO *ssao, Texture *worldPositionTexture, Texture *normalTexture) {
    ShaderBind(&ssao->ssaoShader);

    glBindImageTexture(0, ssao->ssaoSceneTexture.id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R16F);
    glBindImageTexture(1, worldPositionTexture->id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(2, normalTexture->id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
    glBindImageTexture(3, ssao->noiseTexture.id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);

    for (i32 i = 0; i < ssao->kernelSize; i++) {
        std::string str;
        std::sprintf(str.data(), "kernel[%d]", i);
        ShaderSetFloat3(&ssao->ssaoShader, str, ssao->kernel[i]);
    }

    ShaderSetMat4(&ssao->ssaoShader, "view", renderer.camera.view);
    ShaderSetMat4(&ssao->ssaoShader, "proj", renderer.camera.projection);

    ShaderSetFloat(&ssao->ssaoShader, "uRadius", 2.0f);
    ShaderSetFloat(&ssao->ssaoShader, "threshold", 0.02f);

    ShaderSetInt(&ssao->ssaoShader, "kernelSize", ssao->kernelSize);
    ShaderSetInt(&ssao->ssaoShader, "width", renderer.windowWidth);
    ShaderSetInt(&ssao->ssaoShader, "height", renderer.windowHeight);

    glDispatchCompute(renderer.windowWidth / 16, renderer.windowHeight / 16, 1);
    glFinish();

    ShaderBind(&ssao->blurShader);

    glBindImageTexture(0, ssao->ssaoBlurTexture.id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R16F);
    glBindImageTexture(1, ssao->ssaoSceneTexture.id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16F);

    glDispatchCompute(renderer.windowWidth / 16, renderer.windowHeight / 16, 1);
    glFinish();
}
