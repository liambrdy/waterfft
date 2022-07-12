#ifndef SAMPLECOVERAGE_H_
#define SAMPLECOVERAGE_H_

#include "texture.h"
#include "shader.h"

struct SampleCoverage {
    Texture sampleCoverageMask;
    Texture lightScatteringMaskSingleSample;
    Texture specularEmissionBloomMaskSingleSample;

    Texture tmp;

    Shader shader;
};

void SampleCoverageCreate(SampleCoverage *coverage, u32 width, u32 height);
void SampleCoverageRender(SampleCoverage *coverage, Texture *worldPositionTexture, Texture *lightScatteringMask, Texture *specularEmissionBloomMask);

#endif // SAMPLECOVERAGE_H_
