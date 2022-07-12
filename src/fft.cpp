#include "fft.h"

#include <chrono>

void FFTCreate(FFT *fft, i32 N, i32 L, f32 amplitude, f32 alignment, f32 intensity, f32 capillarSupressFactor, const glm::vec2 &direction) {
    fft->N = N;
    fft->log_2_N = std::log(N) / std::log(2);

    TwiddleFactorCreate(&fft->factors, N);
    H0kCreate(&fft->h0k, N, L, amplitude, alignment, intensity, capillarSupressFactor, direction);
    HktCreate(&fft->hkt, N, L);

    TextureCreateStorage(&fft->pingPongTexture, N, N, 1, RGBA32Float, Repeat);
    TextureCreateStorage(&fft->dY, N, N, 1, RGBA32Float, Repeat);
    TextureCreateStorage(&fft->dX, N, N, 1, RGBA32Float, Repeat);
    TextureCreateStorage(&fft->dZ, N, N, 1, RGBA32Float, Repeat);

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/butterfly.comp"};
    info.shaderBits.set(ComputeShader);
    ShaderCreate(&fft->butterflyShader, &info);

    info.shaders[0] = "res/shaders/inversion.comp";
    ShaderCreate(&fft->inversionShader, &info);

    H0kRender(&fft->h0k);
    TwiddleFactorsRender(&fft->factors);
    fft->hkt.h0kImage = fft->h0k.h0kImage;
    fft->hkt.h0minusKImage = fft->h0k.h0MinusKImage;

    fft->begin = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void FFTRender(FFT *fft) {
    HktRender(&fft->hkt, fft->t);

    fft->pingpong = 0;

    ShaderBind(&fft->butterflyShader);

    glBindImageTexture(0, fft->factors.texture.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(1, fft->hkt.dYCoeficcientsImage.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(2, fft->pingPongTexture.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);

    for (i32 i = 0; i < fft->log_2_N; i++) {
        ShaderSetInt(&fft->butterflyShader, "pingpong", fft->pingpong);
        ShaderSetInt(&fft->butterflyShader, "direction", 0);
        ShaderSetInt(&fft->butterflyShader, "stage", i);

        glDispatchCompute(fft->N/16, fft->N/16, 1);
        glFinish();

        fft->pingpong++;
        fft->pingpong %= 2;
    }

    for (i32 i = 0; i < fft->log_2_N; i++) {
        ShaderSetInt(&fft->butterflyShader, "pingpong", fft->pingpong);
        ShaderSetInt(&fft->butterflyShader, "direction", 1);
        ShaderSetInt(&fft->butterflyShader, "stage", i);

        glDispatchCompute(fft->N/16, fft->N/16, 1);
        glFinish();

        fft->pingpong++;
        fft->pingpong %= 2;
    }

    ShaderBind(&fft->inversionShader);

    ShaderSetInt(&fft->inversionShader, "N", fft->N);
    ShaderSetInt(&fft->inversionShader, "pingpong", fft->pingpong);

    glBindImageTexture(0, fft->dY.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);

    glDispatchCompute(fft->N / 16, fft->N / 16, 1);
    glFinish();

    // choppiness
    if (fft->choppy) {
        fft->pingpong = 0;

        ShaderBind(&fft->butterflyShader);

        glBindImageTexture(0, fft->factors.texture.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
        glBindImageTexture(1, fft->hkt.dXCoeficcientsImage.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
        glBindImageTexture(2, fft->pingPongTexture.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);

        for (i32 i = 0; i < fft->log_2_N; i++) {
            ShaderSetInt(&fft->butterflyShader, "pingpong", fft->pingpong);
            ShaderSetInt(&fft->butterflyShader, "direction", 0);
            ShaderSetInt(&fft->butterflyShader, "stage", i);

            glDispatchCompute(fft->N/16, fft->N/16, 1);
            glFinish();

            fft->pingpong++;
            fft->pingpong %= 2;
        }

        for (i32 i = 0; i < fft->log_2_N; i++) {
            ShaderSetInt(&fft->butterflyShader, "pingpong", fft->pingpong);
            ShaderSetInt(&fft->butterflyShader, "direction", 1);
            ShaderSetInt(&fft->butterflyShader, "stage", i);

            glDispatchCompute(fft->N/16, fft->N/16, 1);
            glFinish();

            fft->pingpong++;
            fft->pingpong %= 2;
        }

        ShaderBind(&fft->inversionShader);

        ShaderSetInt(&fft->inversionShader, "N", fft->N);
        ShaderSetInt(&fft->inversionShader, "pingpong", fft->pingpong);

        glBindImageTexture(0, fft->dX.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);

        glDispatchCompute(fft->N / 16, fft->N / 16, 1);
        glFinish();

        fft->pingpong = 0;

        ShaderBind(&fft->butterflyShader);

        glBindImageTexture(0, fft->factors.texture.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
        glBindImageTexture(1, fft->hkt.dZCoeficcientsImage.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
        glBindImageTexture(2, fft->pingPongTexture.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);

        for (i32 i = 0; i < fft->log_2_N; i++) {
            ShaderSetInt(&fft->butterflyShader, "pingpong", fft->pingpong);
            ShaderSetInt(&fft->butterflyShader, "direction", 0);
            ShaderSetInt(&fft->butterflyShader, "stage", i);

            glDispatchCompute(fft->N/16, fft->N/16, 1);
            glFinish();

            fft->pingpong++;
            fft->pingpong %= 2;
        }

        for (i32 i = 0; i < fft->log_2_N; i++) {
            ShaderSetInt(&fft->butterflyShader, "pingpong", fft->pingpong);
            ShaderSetInt(&fft->butterflyShader, "direction", 1);
            ShaderSetInt(&fft->butterflyShader, "stage", i);

            glDispatchCompute(fft->N/16, fft->N/16, 1);
            glFinish();

            fft->pingpong++;
            fft->pingpong %= 2;
        }

        ShaderBind(&fft->inversionShader);

        ShaderSetInt(&fft->inversionShader, "N", fft->N);
        ShaderSetInt(&fft->inversionShader, "pingpong", fft->pingpong);

        glBindImageTexture(0, fft->dZ.id, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);

        glDispatchCompute(fft->N / 16, fft->N / 16, 1);
        glFinish();
    }

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    auto dur = now - fft->begin;
    fft->t += dur * fft->t_delta;
    fft->begin = now;
}
