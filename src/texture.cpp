#include "texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cassert>
#include <random>

static void AllocateImage2D(Texture *texture, u32 internalFormat, u32 format, u32 type) {
    glTexImage2D(texture->target, 0, internalFormat, texture->width, texture->height, 0, format, type, nullptr);
}

static void AllocateImage2D(Texture *texture, u32 internalFormat, u32 format, u32 type, u8 *data) {
    glTexImage2D(texture->target, 0, internalFormat, texture->width, texture->height, 0, format, type, data);
}

static void AllocateImage2DMultisample(Texture *texture, i32 samples, i32 internalFormat) {
    glTexImage2DMultisample(texture->target, samples, internalFormat, texture->width, texture->height, true);
}

static void AllocateStorage2D(Texture *texture, i32 levels, u32 internalFormat) {
    glTexStorage2D(texture->target, levels, internalFormat, texture->width, texture->height);
}

static void SetFilterAndWrapMode(Texture *texture, SamplerFilter filter, TextureWrapMode wrapMode) {
    switch (filter) {
        case Nearest: NearestFilter(texture); break;
        case Bilinear: BilinearFilter(texture); break;
        case Trilinear: TrilinearFilter(texture); break;
        case NoFilter: break;
        default: assert(false);
    }

    switch (wrapMode) {
        case ClampToEdge: ClampToEdgeWrapMode(texture); break;
        case ClampToBorder: ClampToBorderWrapMode(texture); break;
        case Repeat: RepeatWrapMode(texture); break;
        case MirrorRepeat: MirrorRepeatWrapMode(texture); break;
        case NoMode: break;
        default: assert(false);
    }
}

void TextureCreate(Texture *texture, const std::string &path, SamplerFilter filter, TextureWrapMode wrapMode) {
    texture->target = GL_TEXTURE_2D;

    i32 w, h, c;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *pixels = stbi_load(path.c_str(), &w, &h, &c, STBI_default);
    if (!pixels) {
        std::printf("Failed to load texture from file: %s\n", path.c_str());
        std::exit(1);
    }

    texture->width = w;
    texture->height = h;

    u32 format = 0;
    switch (c) {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: {
            std::printf("Unknown loaded texture format: %d from %s\n", c, path.c_str());
            std::exit(1);
        }
    }

    glCreateTextures(texture->target, 1, &texture->id);
    TextureBind(texture);

    glTexImage2D(texture->target, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, pixels);

    SetFilterAndWrapMode(texture, filter, wrapMode);

    stbi_image_free(pixels);
}

void TextureCreate(Texture *texture, u32 width, u32 height, u8 *pixels, ImageFormat format, SamplerFilter filter, TextureWrapMode wrapMode) {
    texture->target = GL_TEXTURE_2D;
    texture->width = width;
    texture->height = height;

    glCreateTextures(texture->target, 1, &texture->id);
    TextureBind(texture);

    switch (format) {
        case RGB8:
            AllocateImage2D(texture, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, pixels); break;
        case RGBA8:
            AllocateImage2D(texture, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, pixels); break;
        default:
            assert(false);
    }

    SetFilterAndWrapMode(texture, filter, wrapMode);
}

void TextureCreate(Texture *texture, u32 width, u32 height, ImageFormat format, SamplerFilter filter, TextureWrapMode wrapMode) {
    texture->target = GL_TEXTURE_2D;
    texture->width = width;
    texture->height = height;

    glCreateTextures(texture->target, 1, &texture->id);
    TextureBind(texture);

    switch (format) {
        case RGBA16Float:
            AllocateImage2D(texture, GL_RGBA16F, GL_RGBA, GL_FLOAT); break;
        case RGBA32Float:
            AllocateImage2D(texture, GL_RGBA32F, GL_RGBA, GL_FLOAT); break;
        case Depth32Float:
            AllocateImage2D(texture, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT); break;
        case R16Float:
            AllocateImage2D(texture, GL_R16F, GL_RED, GL_FLOAT); break;
        case R32Float:
            AllocateImage2D(texture, GL_R32F, GL_RED, GL_FLOAT); break;
        case R8:
            AllocateImage2D(texture, GL_R8, GL_RED, GL_UNSIGNED_BYTE); break;
        default:
            assert(false);
    }

    SetFilterAndWrapMode(texture, filter, wrapMode);
}

void TextureCreate(Texture *texture, u32 width, u32 height, u32 samples, ImageFormat format, SamplerFilter filter, TextureWrapMode wrapMode) {
    if (samples == 1) {
        TextureCreate(texture, width, height, format, filter, wrapMode);
        return ;
    }

    texture->target = GL_TEXTURE_2D_MULTISAMPLE;
    texture->width = width;
    texture->height = height;

    glCreateTextures(texture->target, 1, &texture->id);
    TextureBind(texture);

    switch (format) {
        case RGBA8_SNorm:
            AllocateImage2DMultisample(texture, samples, GL_RGBA8_SNORM); break;
        case RGBA16Float:
            AllocateImage2DMultisample(texture, samples, GL_RGBA16F); break;
        case RGBA32Float:
            AllocateImage2DMultisample(texture, samples, GL_RGBA32F); break;
        case RGB32Float:
            AllocateImage2DMultisample(texture, samples, GL_RGB32F); break;
        case Depth32Float:
            AllocateImage2DMultisample(texture, samples, GL_DEPTH_COMPONENT32F); break;
        case R16Float:
            AllocateImage2DMultisample(texture, samples, GL_R16F); break;
        case R32Float:
            AllocateImage2DMultisample(texture, samples, GL_R32F); break;
        default:
            assert(false);
    }

    SetFilterAndWrapMode(texture, filter, wrapMode);
}

void TextureCreateStorage(Texture *texture, u32 width, u32 height, u32 levels, ImageFormat format, TextureWrapMode wrapMode) {
    texture->target = GL_TEXTURE_2D;
    texture->width = width;
    texture->height = height;

    glCreateTextures(texture->target, 1, &texture->id);
    TextureBind(texture);

    switch (format) {
        case RGBA16Float:
            AllocateStorage2D(texture, levels, GL_RGBA16F); break;
        case RGBA32Float:
            AllocateStorage2D(texture, levels, GL_RGBA32F); break;
        case Depth32Float:
            AllocateStorage2D(texture, levels, GL_DEPTH_COMPONENT32F); break;
        case R16Float:
            AllocateStorage2D(texture, levels, GL_R16F); break;
        case R32Float:
            AllocateStorage2D(texture, levels, GL_R32F); break;
        default:
            assert(false);
    }

    switch (wrapMode) {
        case ClampToEdge: ClampToEdgeWrapMode(texture); break;
        case ClampToBorder: ClampToBorderWrapMode(texture); break;
        case Repeat: RepeatWrapMode(texture); break;
        case MirrorRepeat: MirrorRepeatWrapMode(texture); break;
        case NoMode: break;
        default: assert(false);
    }
}

void TextureBind(Texture *texture) {
    glBindTexture(texture->target, texture->id);
}

void TextureBind(Texture *texture, i32 slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(texture->target, texture->id);
}

void TextureDestroy(Texture *texture) {
    glDeleteTextures(1, &texture->id);
}

std::vector<Texture> GenNoiseTextures(u32 N, u32 count) {
    std::vector<Texture> textures;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, UINT8_MAX);

    for (i32 i = 0; i < count; i++) {
        std::vector<u8> pixels;
        for (i32 y = 0; y < N; y++) {
            for (i32 x = 0; x < N; x++) {
                pixels.push_back(dist(gen));
                pixels.push_back(dist(gen));
                pixels.push_back(0);
            }
        }
        Texture t;
        TextureCreate(&t, N, N, pixels.data(), RGB8, Nearest);
        textures.push_back(t);
    }

    return textures;
}

void NearestFilter(Texture *texture) {
    glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void BilinearFilter(Texture *texture) {
    glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TrilinearFilter(Texture *texture) {
    glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(texture->target);
    glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void ClampToEdgeWrapMode(Texture *texture) {
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ClampToBorderWrapMode(Texture *texture) {
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void RepeatWrapMode(Texture *texture) {
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void MirrorRepeatWrapMode(Texture *texture) {
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}
