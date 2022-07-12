#include "twiddleFactors.h"

#include <vector>
#include <bit>

static uint32_t reverseBits(uint32_t n) {
    uint32_t ans = 0;
    for(int i = 31; i >= 0; i--){
        ans |= (n & 1) <<i;
        n>>=1;
    }
    return ans;
}

static std::vector<i32> genBitReveresedIndices(u32 N) {
    std::vector<i32> indices(N);
    i32 bits = std::log(N) / std::log(2);

    for (i32 i = 0; i < N; i++) {
        u32 x = reverseBits(i);
        x = std::rotl((uint32_t)x, bits);
        indices[i] = x;
    }

    return indices;
}

void TwiddleFactorCreate(TwiddleFactors *factors, u32 N) {
    factors->N = N;

    auto bits = genBitReveresedIndices(N);

    glCreateBuffers(1, &factors->bitReversedSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, factors->bitReversedSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(bits[0]) * bits.size(), bits.data(), GL_STATIC_DRAW);

    factors->log_2_N = (i32)(std::log(N) / std::log(2));

    TextureCreateStorage(&factors->texture, factors->log_2_N, factors->N, 1, RGBA32Float);

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/twiddle.comp"};
    info.shaderBits.set(ComputeShader);
    ShaderCreate(&factors->shader, &info);
}

void TwiddleFactorsRender(TwiddleFactors *factors) {
    ShaderBind(&factors->shader);

    ShaderSetInt(&factors->shader, "N", factors->N);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, factors->bitReversedSSBO);

    glBindImageTexture(0, factors->texture.id, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(factors->log_2_N, factors->N / 16, 1);
}
