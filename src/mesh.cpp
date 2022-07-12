#include "mesh.h"

#include <glad/glad.h>

const std::vector<float> quadVertices = {
-1.0f, -1.0f, 0.0f, 0.0f,
1.0f, -1.0f, 1.0f, 0.0f,
-1.0f, 1.0f, 0.0f, 1.0f,
1.0f, 1.0f, 1.0f, 1.0f,
};

const std::vector<u32> quadIndices = {
0, 2, 1, 1, 2, 3
};

void MeshCreate(Mesh *mesh, const std::vector<glm::vec2> &positions) {
    glCreateVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glCreateBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * positions.size(), positions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    mesh->count = positions.size();
}

void MeshCreate(Mesh *mesh, const std::vector<glm::vec3> &positions) {
    glCreateVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glCreateBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    mesh->count = positions.size();
}

void MeshCreateQuad(Mesh *mesh) {
    glCreateVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glCreateBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)(2 * sizeof(f32)));

    u32 ebo;
    glCreateBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * quadIndices.size(), quadIndices.data(), GL_STATIC_DRAW);

    mesh->count = quadIndices.size();
}

void MeshDestroy(Mesh *mesh) {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
}

void MeshBind(Mesh *mesh) {
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
}

Mesh CreateTerrain(i32 patches) {
    std::vector<glm::vec2> vertices;
    float dx = 1.0f / patches;
    float dy = 1.0f / patches;

    for (float i = 0.0f; i < 1.0f; i += dx) {
        for (float j = 0.0f; j < 1.0f; j += dy) {
            vertices.push_back({i, j});
            vertices.push_back({i+dx*0.33f, j});
            vertices.push_back({i+dx*0.66f, j});
            vertices.push_back({i+dx, j});
            vertices.push_back({i, j+dy*0.33f});
            vertices.push_back({i+dx*0.33f, j+dy*0.33f});
            vertices.push_back({i+dx*0.66f, j+dy*0.33f});
            vertices.push_back({i+dx, j+dy*0.33f});
            vertices.push_back({i, j+dy*0.66f});
            vertices.push_back({i+dx*0.33f, j+dy*0.66f});
            vertices.push_back({i+dx*0.66f, j+dy*0.66f});
            vertices.push_back({i+dx, j+dy*0.66f});
            vertices.push_back({i, j+dy});
            vertices.push_back({i+dx*0.33f, j+dy});
            vertices.push_back({i+dx*0.66f, j+dy});
            vertices.push_back({i+dx, j+dy});
        }
    }

    Mesh m;
    MeshCreate(&m, vertices);

    glPatchParameteri(GL_PATCH_VERTICES, 16);

    return m;
}
