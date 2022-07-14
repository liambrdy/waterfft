#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

#include "input.h"

struct CameraBufferData {
    glm::vec3 eye;
    float padding0;
    glm::mat4 view;
    glm::mat4 viewProj;
    std::array<glm::vec4, 6> frustumPlanes;
};

static glm::vec4 NormalizePlane(const glm::vec4 &plane) {
    glm::vec4 n(1.0f);
    f32 mag = plane.length();
    n = plane / mag;
    return n;
}

static void CreateFrustumPlanes(Camera *camera) {
    glm::vec4 left = glm::vec4(camera->projection[0][3] + camera->projection[0][0]
                               * (float)((std::tan(glm::radians(45.0f / 2))
                               * (camera->width / camera->height))),
                               camera->projection[1][3] + camera->projection[1][0],
                               camera->projection[2][3] + camera->projection[2][0],
                               camera->projection[3][3] + camera->projection[3][0]);
    camera->frustumPlanes[0] = NormalizePlane(left);

    glm::vec4 right = glm::vec4(camera->projection[0][3] - camera->projection[0][0]
                               * (float)((std::tan(glm::radians(45.0f / 2))
                               * (camera->width / camera->height))),
                               camera->projection[1][3] - camera->projection[1][0],
                               camera->projection[2][3] - camera->projection[2][0],
                               camera->projection[3][3] - camera->projection[3][0]);
    camera->frustumPlanes[1] = NormalizePlane(right);

    glm::vec4 bottom = glm::vec4(camera->projection[0][3] + camera->projection[0][1],
                                 camera->projection[1][3] + camera->projection[1][1]
                                 * std::tan(glm::radians(45.0f / 2)),
                                 camera->projection[2][3] + camera->projection[2][1],
                                 camera->projection[3][3] + camera->projection[3][1]);
    camera->frustumPlanes[2] = NormalizePlane(bottom);

    glm::vec4 top = glm::vec4(camera->projection[0][3] - camera->projection[0][1],
                              camera->projection[1][3] - camera->projection[1][1]
                              * std::tan(glm::radians(45.0f / 2)),
                              camera->projection[2][3] - camera->projection[2][1],
                              camera->projection[3][3] - camera->projection[3][1]);
    camera->frustumPlanes[3] = NormalizePlane(top);

    glm::vec4 near = glm::vec4(camera->projection[0][3] + camera->projection[0][2],
                               camera->projection[1][3] + camera->projection[1][2],
                               camera->projection[2][3] + camera->projection[2][2],
                               camera->projection[3][3] + camera->projection[2][3]);
    camera->frustumPlanes[4] = NormalizePlane(near);

    glm::vec4 far = glm::vec4(camera->projection[0][3] - camera->projection[0][2],
                              camera->projection[1][3] - camera->projection[1][2],
                              camera->projection[2][3] - camera->projection[2][2],
                              camera->projection[3][3] - camera->projection[2][3]);
    camera->frustumPlanes[5] = NormalizePlane(far);
}

static void GetCameraBufferData(Camera *camera, CameraBufferData *buffer) {
    buffer->eye = camera->pos;
    buffer->view = camera->view;
    buffer->viewProj = camera->viewProj;
    for (u32 i = 0; i < 6; i++)
        buffer->frustumPlanes[i] = camera->frustumPlanes[i];
}

void CameraCreate(Camera *camera, glm::vec3 eye, float width, float height) {
    camera->pos = eye;
    camera->forward = glm::vec3(0.0f, 0.0f, 1.0f);
    camera->right = glm::vec3(1.0f, 0.0f, 0.0f);
    camera->up = glm::vec3(0.0f, 1.0f, 0.0f);

    camera->width = width;
    camera->height = height;

    camera->view = glm::lookAt(eye, eye + camera->forward, camera->up);
    camera->projection = glm::perspective(glm::radians(45.0f), width / height, 0.01f, 10000.0f);
    camera->viewProj = camera->projection * camera->view;

    CreateFrustumPlanes(camera);

    std::printf("%f, %f, %f, %f\n", camera->frustumPlanes[0].x, camera->frustumPlanes[0].y, camera->frustumPlanes[0].z, camera->frustumPlanes[0].w);

    CameraBufferData bufferData;
    GetCameraBufferData(camera, &bufferData);

    glCreateBuffers(1, &camera->buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, camera->buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(bufferData), &bufferData, GL_DYNAMIC_DRAW);
}

void CameraBind(Camera *camera, Shader *shader) {
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera->buffer);
    ShaderBindUniformBlock(shader, "Camera", 0);
}

void CameraInput(Camera *camera) {
    b8 moved = input.keyDown[KeyA] || input.keyDown[KeyD] || input.keyDown[KeyW] || input.keyDown[KeyS] ||
        input.keyDown[KeyUp] || input.keyDown[KeyDown] || input.keyDown[KeyLeft] || input.keyDown[KeyRight];

    if (input.keyDown[KeyA]) {
        camera->pos += glm::normalize(glm::cross(camera->forward, camera->up)) * 0.5f;
    } else if (input.keyDown[KeyD]) {
        camera->pos -= glm::normalize(glm::cross(camera->forward, camera->up)) * 0.5f;
    }
    if (input.keyDown[KeyW]) {
        camera->pos += camera->forward * 0.5f;
    } else if (input.keyDown[KeyS]) {
        camera->pos -= camera->forward * 0.5f;
    }
    if (input.keyDown[KeyUp]) {
        camera->pos += camera->up * 0.1f;
    } else if (input.keyDown[KeyDown]) {
        camera->pos -= camera->up * 0.1f;
    }

    if (moved)
        CameraUpdateMatrices(camera);
}

void CameraUpdateMatrices(Camera *camera) {
    camera->view = glm::lookAt(camera->pos, camera->pos + camera->forward, camera->up);
    camera->viewProj = camera->projection * camera->view;

    CreateFrustumPlanes(camera);

    CameraBufferData bufferData;
    GetCameraBufferData(camera, &bufferData);

    glBindBuffer(GL_UNIFORM_BUFFER, camera->buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(bufferData), &bufferData);
}

void CameraWindowResize(Camera *camera, float width, float height) {
    camera->projection = glm::perspective(glm::radians(45.0f), width / height, 0.01f, 10000.0f);
}

void CameraDestroy(Camera *camera) {
    glDeleteBuffers(1, &camera->buffer);
}
