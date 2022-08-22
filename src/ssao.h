#ifndef SSAO_H_
#define SSAO_H_

#include "shader.h"
#include "texture.h"

#include <vector>

struct SSAO {
    Texture ssaoBlurTexture;
    Texture ssaoSceneTexture;

    u32 kernelSize;
    std::vector<glm::vec3> kernel;
    std::vector<f32> randomX;
    std::vector<f32> randomY;
    Texture noiseTexture;

    Shader noiseTextureShader;
    Shader ssaoShader;
    Shader blurShader;

    u32 width, height;
};

void SSAOCreate(SSAO *ssao, u32 width, u32 height);
void SSAORender(SSAO *ssao, Texture *worldPositionTexture, Texture *normalTexture);

#endif // SSAO_H_
