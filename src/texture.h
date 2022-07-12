#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "types.h"

#include <glad/glad.h>

#include <string>
#include <vector>

enum ImageFormat {
RGBA8_SNorm,
RGBA8,
RGB8,
RGBA32Float,
RGB32Float,
RGBA16Float,
Depth32Float,
R16Float,
R32Float,
R8
};

enum SamplerFilter {
Nearest,
Bilinear,
Trilinear,
NoFilter
};

enum TextureWrapMode {
ClampToEdge,
ClampToBorder,
Repeat,
MirrorRepeat,
NoMode
};

struct Texture {
    u32 id;
    u32 width, height;
    u32 target;
};

void TextureCreate(Texture *texture, const std::string &path, SamplerFilter filter, TextureWrapMode wrapMode = NoMode);
void TextureCreate(Texture *texture, u32 width, u32 height, u8 *pixels, ImageFormat format, SamplerFilter filter = Nearest, TextureWrapMode wrapMode = NoMode);
void TextureCreate(Texture *texture, u32 width, u32 height, ImageFormat format, SamplerFilter filter = NoFilter, TextureWrapMode wrapMode = NoMode);
void TextureCreate(Texture *texture, u32 width, u32 height, u32 samples, ImageFormat format, SamplerFilter filter = NoFilter, TextureWrapMode wrapMode = NoMode);

void TextureCreateStorage(Texture *texture, u32 width, u32 height, u32 levels, ImageFormat format, TextureWrapMode wrapMode = NoMode);

void TextureBind(Texture *texture);
void TextureBind(Texture *texture, i32 slot);

void TextureDestroy(Texture *texture);

void NearestFilter(Texture *texture);
void BilinearFilter(Texture *texture);
void TrilinearFilter(Texture *texture);

void ClampToEdgeWrapMode(Texture *texture);
void ClampToBorderWrapMode(Texture *texture);
void RepeatWrapMode(Texture *texture);
void MirrorRepeatWrapMode(Texture *texture);

std::vector<Texture> GenNoiseTextures(u32 N, u32 count);

#endif // TEXTURE_H_
