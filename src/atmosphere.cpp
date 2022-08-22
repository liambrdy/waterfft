#include "atmosphere.h"

#include <stb/stb_image.h>

#include <cassert>

#include "renderer.h"

#include <glm/gtc/matrix_transform.hpp>

void AtmosphereCreate(Atmosphere *atmosphere) {
    ShaderCreateInfo info;
    info.shaders = {"res/shaders/atmosphere.vert", "res/shaders/atmosphere.frag"};
    info.shaderBits.set(VertexShader);
    info.shaderBits.set(FragmentShader);
    ShaderCreate(&atmosphere->shader, &info);

    ModelLoad(&atmosphere->dome, "res/models/dome.obj");
    atmosphere->dome.model = glm::scale(glm::mat4(1.0f), glm::vec3(renderer.zFar));
}

void AtmosphereRender(Atmosphere *atmosphere, Camera *camera) {
    ShaderBind(&atmosphere->shader);
    glm::mat4 view = glm::mat4(glm::mat3(camera->view));
    glm::mat4 viewProj = camera->projection * view;

    glm::mat4 model = renderer.world * atmosphere->dome.model;

    ShaderSetMat4(&atmosphere->shader, "viewProj", viewProj);
    ShaderSetMat4(&atmosphere->shader, "model", renderer.world);
    ShaderSetMat4(&atmosphere->shader, "modelViewProj", viewProj * model);

    ShaderSetInt(&atmosphere->shader, "width", renderer.windowWidth);
    ShaderSetInt(&atmosphere->shader, "height", renderer.windowHeight);

    ShaderSetInt(&atmosphere->shader, "isReflection", renderer.isReflection);

    ModelRender(&atmosphere->dome);
}
