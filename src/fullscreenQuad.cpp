#include "fullscreenQuad.h"

#include "renderer.h"

void FullscreenQuadCreate(FullscreenQuad *quad) {
    MeshCreateQuad(&quad->quad);

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/quad.vert", "res/shaders/quad.frag"};
    info.shaderBits.set(VertexShader);
    info.shaderBits.set(FragmentShader);
    ShaderCreate(&quad->shader, &info);

    info.shaders[1] = "res/shaders/quadMS.frag";
    ShaderCreate(&quad->shaderMS, &info);
}

void FullscreenQuadRender(FullscreenQuad *quad, Texture *texture) {
    ShaderBind(&quad->shader);

    ShaderSetInt(&quad->shader, "tex", 0);
    TextureBind(texture, 0);

    MeshBind(&quad->quad);

    glDrawElements(GL_TRIANGLES, quad->quad.count, GL_UNSIGNED_INT, nullptr);
}


void FullscreenQuadRenderMS(FullscreenQuad *quad, Texture *texture) {
    ShaderBind(&quad->shaderMS);

    ShaderSetInt(&quad->shaderMS, "multisamples", renderer.samples);
    ShaderSetInt(&quad->shaderMS, "width", renderer.windowWidth);
    ShaderSetInt(&quad->shaderMS, "height", renderer.windowHeight);

    ShaderSetInt(&quad->shaderMS, "tex", 0);
    TextureBind(texture, 0);

    MeshBind(&quad->quad);

    glDrawElements(GL_TRIANGLES, quad->quad.count, GL_UNSIGNED_INT, nullptr);
}
