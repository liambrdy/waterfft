#ifndef FULLSCREENQUAD_H_
#define FULLSCREENQUAD_H_

#include "shader.h"
#include "texture.h"
#include "mesh.h"

struct FullscreenQuad {
    Shader shader, shaderMS;

    Mesh quad;
};

void FullscreenQuadCreate(FullscreenQuad *quad);
void FullscreenQuadRender(FullscreenQuad *quad, Texture *texture);
void FullscreenQuadRenderMS(FullscreenQuad *quad, Texture *texture);

#endif // MUTLTISAMPLEQUAD_H_
