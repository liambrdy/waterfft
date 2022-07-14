#include "renderer.h"

Renderer renderer;

void RendererInit(u32 width, u32 height) {
    renderer.windowWidth = width;
    renderer.windowHeight = height;

    CameraCreate(&renderer.camera, glm::vec3(0.0f, 3.0f, 0.0f), width, height);
    renderer.world = glm::mat4(1.0f);

    renderer.samples = 1;
    OffscreenFramebufferCreate(&renderer.primaryFbo, width, height, renderer.samples);

    SampleCoverageCreate(&renderer.sampleCoverage, width, height);

    FullscreenQuadCreate(&renderer.quad);

    renderer.light.direction = glm::normalize(glm::vec3(0.6527264f, -0.42568117f, 0.62669265f));
    renderer.light.ambient = glm::vec3(0.01f);
    renderer.light.color = glm::vec3(1.0f, 0.95f, 0.87f);
    renderer.light.intensity = 1.0f;
    DeferredLightingCreate(&renderer.lighting);
}

void RendererBeginFrame() {
    OffscreenFramebufferBind(&renderer.primaryFbo);
    FramebufferClear();

    CameraInput(&renderer.camera);
}

void RendererEndFrame() {
    glFinish();

    FramebufferUnbind();

    SampleCoverageRender(&renderer.sampleCoverage,
                         &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::Position),
                         &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::LightScattering),
                         &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::SpecularEmissionDiffuseSSAOBloom));

    DeferredLightingRender(&renderer.lighting, &renderer.sampleCoverage.sampleCoverageMask,
                           &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::Color),
                           &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::Position),
                           &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::Normal),
                           &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::SpecularEmissionDiffuseSSAOBloom));

    //FullscreenQuadRenderMS(&renderer.quad, &OffscreenFramebufferGetTexture(&renderer.primaryFbo, Attachment::Position));
    FullscreenQuadRender(&renderer.quad, &renderer.lighting.deferredLightingSceneTexture);
}
