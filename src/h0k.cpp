#include "h0k.h"

void H0kCreate(H0k *h0k, u32 N, u32 L, float amplitude, float alignment, float intensity, float capillarSupressFactor, const glm::vec2 &direction) {
    h0k->N = N;
    h0k->L = L;
    h0k->amplitude = amplitude;
    h0k->alignment = alignment;
    h0k->intensity = intensity;
    h0k->capillarSupressFactor = capillarSupressFactor;
    h0k->direction = direction;

    h0k->noise = GenNoiseTextures(N, 4);

    TextureCreateStorage(&h0k->h0kImage, N, N, 1, RGBA32Float);
    TextureCreateStorage(&h0k->h0MinusKImage, N, N, 1, RGBA32Float);

    ShaderCreateInfo info = {};
    info.shaders = {"res/shaders/h0k.comp"};
    info.shaderBits.set(ComputeShader);

    ShaderCreate(&h0k->shader, &info);
}

void H0kRender(H0k *h0k) {
    ShaderBind(&h0k->shader);

    ShaderSetInt(&h0k->shader, "N", h0k->N);
    ShaderSetInt(&h0k->shader, "L", h0k->L);
    ShaderSetFloat(&h0k->shader, "amplitude", h0k->amplitude);
    ShaderSetFloat(&h0k->shader, "alignment", h0k->alignment);
    ShaderSetFloat(&h0k->shader, "intensity", h0k->intensity);
    ShaderSetFloat(&h0k->shader, "l", h0k->capillarSupressFactor);
    ShaderSetFloat2(&h0k->shader, "direction", h0k->direction);

    for (i32 i = 0; i < 4; i++)
        TextureBind(&h0k->noise[i], i);

    ShaderSetInt(&h0k->shader, "noise_r0", 0);
    ShaderSetInt(&h0k->shader, "noise_i0", 1);
    ShaderSetInt(&h0k->shader, "noise_r1", 2);
    ShaderSetInt(&h0k->shader, "noise_i1", 3);

    glBindImageTexture(0, h0k->h0kImage.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindImageTexture(1, h0k->h0MinusKImage.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(h0k->N/16, h0k->N/16, 1);
}
