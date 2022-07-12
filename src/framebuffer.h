#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "types.h"
#include "texture.h"


struct Framebuffer {
    u32 id;
};

void FramebufferCreate(Framebuffer *framebuffer);

void FramebufferCreateColorAttachment(Framebuffer *framebuffer, Texture *texture, u32 attachment, b8 multisample = false);
void FramebufferCreateColorAttachment(Framebuffer *framebuffer, u32 w, u32 h, u32 attachment, u32 format);

void FramebufferCreateDepthAttachment(Framebuffer *framebuffer, Texture *texture, b8 multisample = false);
void FramebufferCreateDepthAttachment(Framebuffer *framebuffer, u32 w, u32 h);

void FramebufferSetDrawBuffers(std::vector<u32> drawBuffers);

void FramebufferBind(Framebuffer *framebuffer);
void FramebufferUnbind();

void FramebufferClear();

void FramebufferCheckStatus(Framebuffer *framebuffer);

#endif // FRAMEBUFFER_H_
