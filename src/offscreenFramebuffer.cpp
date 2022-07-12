#include "offscreenFramebuffer.h"

#include <cassert>

void OffscreenFramebufferCreate(OffscreenFramebuffer *fbo, u32 width, u32 height, u32 samples) {
    Texture albedoAttachment, worldPositionAttachment, normalAttachment, specularEmissionDiffuseSsaoBloomAttachment, lightScatteringAttachment, depthAttachment;

    TextureCreate(&albedoAttachment, width, height, samples, RGBA16Float, Nearest, ClampToEdge);
    TextureCreate(&worldPositionAttachment, width, height, samples, RGBA32Float, Nearest, ClampToEdge);
    TextureCreate(&normalAttachment, width, height, samples, RGBA16Float, Nearest, ClampToEdge);
    TextureCreate(&specularEmissionDiffuseSsaoBloomAttachment, width, height, samples, RGBA16Float, Nearest, ClampToEdge);
    TextureCreate(&lightScatteringAttachment, width, height, samples, RGBA16Float, Nearest, ClampToEdge);
    TextureCreate(&depthAttachment, width, height, samples, Depth32Float, Nearest, ClampToEdge);

    fbo->attachments[(size_t)Attachment::Color] = albedoAttachment;
    fbo->attachments[(size_t)Attachment::Position] = worldPositionAttachment;
    fbo->attachments[(size_t)Attachment::Normal] = normalAttachment;
    fbo->attachments[(size_t)Attachment::SpecularEmissionDiffuseSSAOBloom] = specularEmissionDiffuseSsaoBloomAttachment;
    fbo->attachments[(size_t)Attachment::LightScattering] = lightScatteringAttachment;
    fbo->attachments[(size_t)Attachment::Depth] = depthAttachment;

    FramebufferCreate(&fbo->fbo);

    b8 ms = samples > 1;

    FramebufferCreateColorAttachment(&fbo->fbo, &albedoAttachment, 0, ms);
    FramebufferCreateColorAttachment(&fbo->fbo, &worldPositionAttachment, 1, ms);
    FramebufferCreateColorAttachment(&fbo->fbo, &normalAttachment, 2, ms);
    FramebufferCreateColorAttachment(&fbo->fbo, &specularEmissionDiffuseSsaoBloomAttachment, 3, ms);
    FramebufferCreateColorAttachment(&fbo->fbo, &lightScatteringAttachment, 4, ms);
    FramebufferCreateDepthAttachment(&fbo->fbo, &depthAttachment, ms);

    std::vector<u32> drawBuffers;
    drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT1);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT2);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT3);
    drawBuffers.push_back(GL_COLOR_ATTACHMENT4);

    FramebufferSetDrawBuffers(drawBuffers);
    FramebufferCheckStatus(&fbo->fbo);
    FramebufferUnbind();
}

void OffscreenFramebufferBind(OffscreenFramebuffer *fbo) {
    FramebufferBind(&fbo->fbo);
}

void OffscreenFramebufferUnbind() {
    FramebufferUnbind();
}

Texture &OffscreenFramebufferGetTexture(OffscreenFramebuffer *fbo, Attachment attachment) {
    assert(attachment >= Attachment::Color && attachment < Attachment::Count);
    return fbo->attachments[(size_t)attachment];
}
