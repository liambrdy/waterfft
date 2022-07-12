#ifndef OFFSCREENFRAMEBUFFER_H_
#define OFFSCREENFRAMEBUFFER_H_

#include "types.h"
#include "framebuffer.h"
#include "texture.h"

#include <array>

enum class Attachment {
    Color,
    Position,
    Normal,
    SpecularEmissionDiffuseSSAOBloom,
    LightScattering,
    Depth,
    Count
};

struct OffscreenFramebuffer {
    Framebuffer fbo;

    std::array<Texture, (size_t)Attachment::Count> attachments;
};

void OffscreenFramebufferCreate(OffscreenFramebuffer *fbo, u32 width, u32 height, u32 samples);

void OffscreenFramebufferBind(OffscreenFramebuffer *fbo);
void OffscreenFramebufferUnbind();

Texture &OffscreenFramebufferGetTexture(OffscreenFramebuffer *fbo, Attachment attachment);

#endif // OFFSCREENFRAMEBUFFER_H_
