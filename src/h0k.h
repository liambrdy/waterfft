#ifndef H0K_H_
#define H0K_H_

#include "types.h"
#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

struct H0k {
    u32 N, L;
    float amplitude, alignment, intensity, capillarSupressFactor;
    glm::vec2 direction;
    Shader shader;

    std::vector<Texture> noise;

    Texture h0kImage, h0MinusKImage;
};

void H0kCreate(H0k *h0k, u32 N, u32 L, float amplitude, float alignment, float intensity, float capillarSupressFactor, const glm::vec2 &direction);

void H0kRender(H0k *h0k);

#endif // H0K_H_
