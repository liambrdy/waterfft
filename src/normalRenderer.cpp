#include "normalRenderer.h"

void NormalRendererCreate(NormalRenderer *renderer, u32 N) {
    renderer->N = N;
    renderer->strength = 5.0f;

    TextureCreateStorage(&renderer->normalMap, N, N, std::log(N) / std::log(2), RGBA32Float);
    TrilinearFilter(&renderer->normalMap);

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/normals.comp"};
    info.shaderBits.set(ComputeShader);
    ShaderCreate(&renderer->shader, &info);
}

void NormalRendererRender(NormalRenderer *renderer, Texture *heightMap) {
    ShaderBind(&renderer->shader);

    ShaderSetInt(&renderer->shader, "N", renderer->N);
    ShaderSetFloat(&renderer->shader, "strength", renderer->strength);

    TextureBind(heightMap, 0);
    ShaderSetInt(&renderer->shader, "heightMap", 0);

    glBindImageTexture(0, renderer->normalMap.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(renderer->N / 16, renderer->N / 16, 1);
    glFinish();

    TextureBind(&renderer->normalMap);
    TrilinearFilter(&renderer->normalMap);
}
