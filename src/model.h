#ifndef MODEL_H_
#define MODEL_H_

#include "shader.h"

#include <vector>

struct ModelVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Mesh;

struct Model {
    std::vector<Mesh> meshes;
    glm::mat4 model;
};

void ModelLoad(Model *model, const char *modelPath);
void ModelSetUniforms(Model *model, Shader *shader, b8 setCamera = true);
void ModelRender(Model *model);

#endif // MODEL_H_
