#ifndef DEFERREDLIGHTING_H_
#define DEFERREDLIGHTING_H_

#include "texture.h"
#include "shader.h"

struct DeferredLighting {
    Texture deferredLightingSceneTexture;
    Shader shader;

    u32 directionalLightUbo;
};

void DeferredLightingCreate(DeferredLighting *lighting);
void DeferredLightingRender(DeferredLighting *lighting, Texture *sampleCoverage, Texture *ssaoBlur, Texture *albedo, Texture *worldPosition, Texture *normal, Texture *specularEmissionDiffuseSsaoBloom);

#endif // DEFERREDLIGHTING_H_
