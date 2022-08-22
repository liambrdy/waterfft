#ifndef UTIL_H_
#define UTIL_H_

#include <random>

#include "types.h"

#include <glm/glm.hpp>

struct RandomEngine {
    std::random_device rd;
    std::mt19937 gen;

    RandomEngine()
        : gen(rd())
    {}

    f32 rndF32(f32 min, f32 max) {
        std::uniform_real_distribution<f32> d(min, max);
        return d(gen);
    }

    i32 rndI32(i32 min, i32 max) {
        std::uniform_int_distribution<i32> d(min, max);
        return d(gen);
    }

    u32 rndU32(u32 min, u32 max) {
        std::uniform_int_distribution<u32> d(min, max);
        return d(gen);
    }
};

extern RandomEngine rndEngine;

std::vector<glm::vec3> GenerateRandomKernel3D(i32 kernelSize);

#endif // UTIL_H_
