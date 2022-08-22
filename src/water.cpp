#include "water.h"

#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>

struct WaterSsbo {
    glm::mat4 worldTransform;
    i32 uvScale;
    i32 tessFactor;
    f32 tessSlope;
    f32 tessShift;
    f32 displacementScale;
    i32 highDetailRange;
    f32 choppiness;
    f32 kReflection;
    f32 kRefraction;
    i32 windowWidth;
    i32 windowHeight;
    f32 reflectionBlendFactor;
    glm::vec3 waterColor;
    f32 fresnelFactor;
    f32 capillarStrength;
    f32 capillarDownsampling;
    f32 dudvDownsampling;
};

static void GetWaterBuffer(Water *water, WaterSsbo *ssbo) {
    ssbo->worldTransform = water->transform;
    ssbo->uvScale = water->waterConfig.uvScale;
    ssbo->tessFactor = water->waterConfig.tessellationFactor;
    ssbo->tessSlope = water->waterConfig.tessellationSlope;
    ssbo->tessShift = water->waterConfig.tessellationShift;
    ssbo->displacementScale = water->waterConfig.displacementScale;
    ssbo->highDetailRange = water->waterConfig.highDetailRange;
    ssbo->choppiness = water->waterConfig.choppiness;
    ssbo->kReflection = water->waterConfig.kReflection;
    ssbo->kRefraction = water->waterConfig.kRefraction;
    ssbo->windowWidth = renderer.windowWidth;
    ssbo->windowHeight = renderer.windowHeight;
    ssbo->reflectionBlendFactor = water->waterConfig.reflectionBlendFactor;
    ssbo->waterColor = water->waterConfig.baseColor;
    ssbo->fresnelFactor = water->waterConfig.fresnelFactor;
    ssbo->capillarStrength = water->waterConfig.capillarStrength;
    ssbo->capillarDownsampling = water->waterConfig.capillarDownsampling;
    ssbo->dudvDownsampling = water->waterConfig.dudvDownsampling;
}

void WaterCreate(Water *water, u32 patches) {
    FFTCreate(&water->fft, water->waterConfig.N, water->waterConfig.L, water->waterConfig.amplitude, water->waterConfig.alignment, water->waterConfig.windSpeed, water->waterConfig.capillarWaveSupression, water->waterConfig.windDirection);
    water->fft.choppy = water->waterConfig.choppy;
    water->fft.t_delta = water->waterConfig.t_delta;

    NormalRendererCreate(&water->normalRenderer, water->waterConfig.N);
    water->normalRenderer.strength = water->waterConfig.normalStrength;

    TextureCreate(&water->dudvTexture, "res/textures/dudv1.jpg", Trilinear);

    u32 w = renderer.windowWidth;
    u32 h = renderer.windowHeight;

    std::vector<u32> drawBuffers = {GL_COLOR_ATTACHMENT0};

    TextureCreate(&water->reflectionTexture, w/2, h/2, RGBA16Float, Nearest);

    FramebufferCreate(&water->reflectionFbo);
    FramebufferCreateColorAttachment(&water->reflectionFbo, &water->reflectionTexture, 0);
    FramebufferCreateDepthAttachment(&water->reflectionFbo, w/2, h/2);
    FramebufferSetDrawBuffers(drawBuffers);
    FramebufferCheckStatus(&water->reflectionFbo);

    TextureCreate(&water->refractionTexture, w/2, h/2, RGBA16Float, Nearest);

    Framebuffer refractionFbo;
    FramebufferCreate(&water->refractionFbo);
    FramebufferCreateColorAttachment(&water->refractionFbo, &water->refractionTexture, 0);
    FramebufferCreateDepthAttachment(&water->refractionFbo, w/2, h/2);
    FramebufferSetDrawBuffers(drawBuffers);
    FramebufferCheckStatus(&water->refractionFbo);

    FramebufferUnbind();

    ShaderCreateInfo info = {};
    info.shaders = {"res/shaders/water.vert", "res/shaders/water.tesc", "res/shaders/water.tese", "res/shaders/water.geom", "res/shaders/water.frag"};
    info.shaderBits.set(VertexShader);
    info.shaderBits.set(TessellationControlShader);
    info.shaderBits.set(TessellationEvalShader);
    info.shaderBits.set(GeometryShader);
    info.shaderBits.set(FragmentShader);
    ShaderCreate(&water->shader, &info);

    water->plane = CreateTerrain(patches);

    WaterSsbo buffer;
    GetWaterBuffer(water, &buffer);

    glCreateBuffers(1, &water->ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, water->ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(WaterSsbo), &buffer, GL_STATIC_DRAW);

    water->begin = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    water->motion = 0.0f;
    water->distortion = 0.0f;
}

void WaterUpdate(Water *water) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    auto dur = now - water->begin;
    water->motion += dur * water->waterConfig.waveMotion_delta;
    water->distortion += dur * water->waterConfig.distortion_delta;
    water->begin = now;
}

void WaterRender(Water *water, Camera *camera, std::function<void()> renderFunc) {
    glEnable(GL_CLIP_DISTANCE6);

    glViewport(0, 0, renderer.windowWidth/2, renderer.windowHeight/2);

    renderer.world = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
    renderer.isReflection = 1;

    FramebufferBind(&water->reflectionFbo);
    FramebufferClear();
    glFrontFace(GL_CCW);

    renderFunc();

    glFinish();
    glFrontFace(GL_CW);
    FramebufferUnbind();

    renderer.world = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    renderer.isReflection = 0;

    FramebufferBind(&water->refractionFbo);
    FramebufferClear();

    renderFunc();

    glFinish();
    FramebufferUnbind();

    OffscreenFramebufferBind(&renderer.primaryFbo);

    glDisable(GL_CLIP_DISTANCE6);
    glViewport(0, 0, renderer.windowWidth, renderer.windowHeight);

    FFTRender(&water->fft);
    NormalRendererRender(&water->normalRenderer, &water->fft.dY);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, water->ssbo);

    ShaderBind(&water->shader);

    TextureBind(&water->fft.dY, 0);
    TextureBind(&water->fft.dX, 1);
    TextureBind(&water->fft.dZ, 2);
    TextureBind(&water->normalRenderer.normalMap, 3);
    TextureBind(&water->reflectionTexture, 4);
    TextureBind(&water->refractionTexture, 5);
    TextureBind(&water->dudvTexture, 6);

    ShaderSetInt(&water->shader, "dY", 0);
    ShaderSetInt(&water->shader, "dX", 1);
    ShaderSetInt(&water->shader, "dZ", 2);
    ShaderSetInt(&water->shader, "normalMap", 3);
    ShaderSetInt(&water->shader, "waterReflection", 4);
    ShaderSetInt(&water->shader, "waterRefraction", 5);
    ShaderSetInt(&water->shader, "dudvMap", 6);

    ShaderSetFloat(&water->shader, "motion", water->motion);
    ShaderSetFloat(&water->shader, "distortion", water->distortion);
    ShaderSetFloat2(&water->shader, "wind", water->waterConfig.windDirection);

    CameraBind(camera, &water->shader);
    MeshBind(&water->plane);

    glDrawArrays(GL_PATCHES, 0, water->plane.count);

    glFinish();
}
