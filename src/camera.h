#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

#include "types.h"
#include "shader.h"

#include <array>

struct Camera {
    glm::vec3 pos;
    glm::mat4 view;
    u32 buffer;

    glm::mat4 projection;
    glm::mat4 viewProj;
    std::array<glm::vec4, 6> frustumPlanes;

    f32 width, height;

    glm::vec3 forward, right, up;
};

void CameraCreate(Camera *camera, glm::vec3 eye, float width, float height);
void CameraBind(Camera *camera, Shader *shader);

void CameraInput(Camera *camera);

void CameraUpdateMatrices(Camera *camera);
void CameraWindowResize(Camera *camera, float width, float height);

void CameraDestroy(Camera *camera);

#endif // CAMERA_H_
