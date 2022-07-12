#include "hkt.h"

void HktCreate(Hkt *hkt, u32 N, u32 L) {
    hkt->N = N;
    hkt->L = L;

    TextureCreateStorage(&hkt->dYCoeficcientsImage, N, N, 1, RGBA32Float);
    TextureCreateStorage(&hkt->dXCoeficcientsImage, N, N, 1, RGBA32Float);
    TextureCreateStorage(&hkt->dZCoeficcientsImage, N, N, 1, RGBA32Float);

    ShaderCreateInfo info = {};
    info.shaders = {"res/shaders/hkt.comp"};
    info.shaderBits.set(ComputeShader);
    ShaderCreate(&hkt->shader, &info);
}

void HktRender(Hkt *hkt, f32 t) {
    ShaderBind(&hkt->shader);

    ShaderSetInt(&hkt->shader, "N", hkt->N);
    ShaderSetInt(&hkt->shader, "L", hkt->L);
    ShaderSetFloat(&hkt->shader, "t", t);

    glBindImageTexture(0, hkt->dYCoeficcientsImage.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(1, hkt->dXCoeficcientsImage.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(2, hkt->dZCoeficcientsImage.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(3, hkt->h0kImage.id, 0, false, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(4, hkt->h0minusKImage.id, 0, false, 0, GL_READ_ONLY, GL_RGBA32F);

    glDispatchCompute(hkt->N / 16, hkt->N / 16, 1);
    glFinish();
}
