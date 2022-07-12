#ifndef TWIDDLEFACTORS_H_
#define TWIDDLEFACTORS_H_

#include "types.h"
#include "texture.h"
#include "shader.h"

struct TwiddleFactors {
    u32 N;
    i32 log_2_N;

    Texture texture;
    Shader shader;

    u32 bitReversedSSBO;
};

void TwiddleFactorCreate(TwiddleFactors *factors, u32 N);
void TwiddleFactorsRender(TwiddleFactors *factors);

#endif // TWIDDLEFACTORS_H_
