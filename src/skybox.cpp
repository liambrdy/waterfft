#include "skybox.h"

#include <stb/stb_image.h>

#include <cassert>

#include "renderer.h"

const std::vector<glm::vec3> skyboxVertices = {
    // positions
{-1.0f,  1.0f, -1.0f},
{-1.0f, -1.0f, -1.0f},
{1.0f, -1.0f, -1.0f},
{1.0f, -1.0f, -1.0f},
{1.0f,  1.0f, -1.0f},
{-1.0f,  1.0f, -1.0f},

{-1.0f, -1.0f,  1.0f},
{-1.0f, -1.0f, -1.0f},
{-1.0f,  1.0f, -1.0f},
{-1.0f,  1.0f, -1.0f},
{-1.0f,  1.0f,  1.0f},
{-1.0f, -1.0f,  1.0f},

{1.0f, -1.0f, -1.0f},
{1.0f, -1.0f,  1.0f},
{1.0f,  1.0f,  1.0f},
{1.0f,  1.0f,  1.0f},
{1.0f,  1.0f, -1.0f},
{1.0f, -1.0f, -1.0f},

{-1.0f, -1.0f,  1.0f},
{-1.0f,  1.0f,  1.0f},
{1.0f,  1.0f,  1.0f},
{1.0f,  1.0f,  1.0f},
{1.0f, -1.0f,  1.0f},
{-1.0f, -1.0f,  1.0f},

{-1.0f,  1.0f, -1.0f},
{1.0f,  1.0f, -1.0f},
{1.0f,  1.0f,  1.0f},
{1.0f,  1.0f,  1.0f},
{-1.0f,  1.0f,  1.0f},
{-1.0f,  1.0f, -1.0f},

{-1.0f, -1.0f, -1.0f},
{-1.0f, -1.0f,  1.0f},
{1.0f, -1.0f, -1.0f},
{1.0f, -1.0f, -1.0f},
{-1.0f, -1.0f,  1.0f},
{1.0f, -1.0f,  1.0}
};

void SkyboxCreate(Skybox *skybox, const std::vector<std::string> &paths) {
    MeshCreate(&skybox->box, skyboxVertices);

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &skybox->texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);

    i32 width, height, channels;
    for (u32 i = 0; i < paths.size(); i++) {
        stbi_uc *pixels = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_default);
        assert(pixels && "Failed to load skybox");

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_image_free(pixels);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    ShaderCreateInfo info;
    info.shaders = {"res/shaders/skybox.vert", "res/shaders/skybox.frag"};
    info.shaderBits.set(VertexShader);
    info.shaderBits.set(FragmentShader);
    ShaderCreate(&skybox->shader, &info);
}

void SkyboxRender(Skybox *skybox, Camera *camera) {
    glDepthFunc(GL_LEQUAL);

    ShaderBind(&skybox->shader);
    glm::mat4 view = glm::mat4(glm::mat3(camera->view));
    glm::mat4 viewProj = camera->projection * camera->view;

    ShaderSetMat4(&skybox->shader, "viewProj", viewProj);
    ShaderSetMat4(&skybox->shader, "world", renderer.world);

    ShaderSetInt(&skybox->shader, "width", renderer.windowWidth);
    ShaderSetInt(&skybox->shader, "height", renderer.windowHeight);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);

    MeshBind(&skybox->box);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}
