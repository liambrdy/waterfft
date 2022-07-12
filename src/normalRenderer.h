#ifndef NORMALRENDERER_H_
#define NORMALRENDERER_H_

#include "types.h"
#include "shader.h"
#include "texture.h"

struct NormalRenderer {
    u32 N;
    f32 strength;

    Shader shader;
    Texture normalMap;
};

void NormalRendererCreate(NormalRenderer *renderer, u32 N);

void NormalRendererRender(NormalRenderer *renderer, Texture *heightMap);

#endif // NORMALRENDERER_H_
