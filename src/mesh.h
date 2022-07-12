#ifndef MESH_H_
#define MESH_H_

#include <glm/glm.hpp>

#include <vector>

#include "types.h"

struct Mesh {
    u32 vao, vbo;
    u32 count;
};

void MeshCreate(Mesh *mesh, const std::vector<glm::vec2> &positions);
void MeshCreate(Mesh *mesh, const std::vector<glm::vec3> &positions);

void MeshCreateQuad(Mesh *mesh);

void MeshDestroy(Mesh *mesh);

void MeshBind(Mesh *mesh);

Mesh CreateTerrain(i32 patches);

#endif // MESH_H_
