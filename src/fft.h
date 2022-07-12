#ifndef FFT_H_
#define FFT_H_

#include "types.h"
#include "texture.h"
#include "shader.h"

#include "h0k.h"
#include "hkt.h"
#include "twiddleFactors.h"

struct FFT {
    Texture dX, dY, dZ;
    b8 choppy;

    Texture pingPongTexture;

    i32 log_2_N;
    i32 pingpong;
    i32 N;

    Shader butterflyShader, inversionShader;

    TwiddleFactors factors;
    H0k h0k;
    Hkt hkt;

    long begin;
    f32 t;
    f32 t_delta;
};

void FFTCreate(FFT *fft, i32 N, i32 L, f32 amplitude, f32 alignment, f32 intensity, f32 capillarSupressFactor, const glm::vec2 &direction);

void FFTRender(FFT *fft);

#endif // FFT_H_
