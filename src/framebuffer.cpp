#include "framebuffer.h"

void FramebufferCreate(Framebuffer *framebuffer) {
    glCreateFramebuffers(1, &framebuffer->id);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
}

void FramebufferCreateColorAttachment(Framebuffer *framebuffer, Texture *texture, u32 attachment, b8 multisample) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture->id, 0);
}

void FramebufferCreateColorAttachment(Framebuffer *framebuffer, u32 w, u32 h, u32 attachment, u32 format) {
    u32 rb;
    glCreateRenderbuffers(1, &rb);
    glBindRenderbuffer(GL_RENDERBUFFER, rb);
    glRenderbufferStorage(GL_RENDERBUFFER, format, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_RENDERBUFFER, rb);
}

void FramebufferCreateDepthAttachment(Framebuffer *framebuffer, Texture *texture, b8 multisample) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture->id, 0);
}

void FramebufferCreateDepthAttachment(Framebuffer *framebuffer, u32 w, u32 h) {
    u32 rb;
    glCreateRenderbuffers(1, &rb);
    glBindRenderbuffer(GL_RENDERBUFFER, rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);
}

void FramebufferSetDrawBuffers(std::vector<u32> drawBuffers) {
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());
}

void FramebufferBind(Framebuffer *framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
}

void FramebufferUnbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferClear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FramebufferCheckStatus(Framebuffer *framebuffer) {
    u32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE: return;
        case GL_FRAMEBUFFER_UNDEFINED: std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_UNDEFINED\n"); std::exit(1);
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"); std::exit(1);
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:  std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"); std::exit(1);
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n"); std::exit(1);
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n"); std::exit(1);
        case GL_FRAMEBUFFER_UNSUPPORTED: std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_UNSUPPORTED\n"); std::exit(1);
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n"); std::exit(1);
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: std::printf("Framebuffer creation failed with GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n"); std::exit(1);
    }
}
