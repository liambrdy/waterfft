#ifndef RENDERER_H_
#define RENDERER_H_

#include "types.h"
#include "framebuffer.h"
#include "offscreenFramebuffer.h"
#include "sampleCoverage.h"
#include "camera.h"

#include "fullscreenQuad.h"
#include "deferredLighting.h"

#include <glm/glm.hpp>

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 color;
    f32 intensity;
};

struct Renderer {
    u32 windowWidth, windowHeight;

    Camera camera;
    glm::mat4 world;

    OffscreenFramebuffer primaryFbo;
    u32 samples;

    SampleCoverage sampleCoverage;

    FullscreenQuad quad;

    DeferredLighting lighting;
    DirectionalLight light;
};

extern Renderer renderer;

void RendererInit(u32 width, u32 height);

void RendererBeginFrame();
void RendererEndFrame();

#endif // RENDERER_H_
