#include "util.h"

RandomEngine rndEngine;

std::vector<glm::vec3> GenerateRandomKernel3D(i32 kernelSize) {
    std::vector<glm::vec3> kernel(kernelSize);

    for (i32 i = 0; i < kernelSize; i++) {
        glm::vec3 k(rndEngine.rndF32(0.0f, 1.0f), rndEngine.rndF32(0.0f, 1.0f), rndEngine.rndF32(0.0f, 1.0f));
        k = glm::normalize(k);

        f32 scale = (f32)i / (f32)kernelSize;
        scale = (f32)std::min(std::max(0.01f, scale*scale), 1.0f);

        k *= scale * -1.0f;
        kernel[i] = k;
    }

    return kernel;
}
