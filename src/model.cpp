#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "renderer.h"
#include "mesh.h"

static Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<ModelVertex> vertices;
    std::vector<u32> indices;

    for (u32 i = 0; i < mesh->mNumVertices; i++) {
        ModelVertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.uv = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (u32 i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh m;
    MeshCreate(&m, vertices, indices);

    return m;
}

static void processNode(Model *model, aiNode *node, const aiScene *scene) {
    for (u32 i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(processMesh(mesh, scene));
    }

    for (u32 i = 0; i < node->mNumChildren; i++) {
        processNode(model, node->mChildren[i], scene);
    }
}

void ModelLoad(Model *model, const char *modelPath) {
    const aiScene *scene = renderer.importer.ReadFile(modelPath, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices);

    if (!scene) {
        std::printf("Failed to load model: %s\n", renderer.importer.GetErrorString());
        std::exit(1);
    }

    processNode(model, scene->mRootNode, scene);

    model->model = glm::mat4(1.0f);
}

#define GLM_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void ModelSetUniforms(Model *model, Shader *shader, b8 setCamera) {
    ShaderSetMat4(shader, "model", renderer.world * model->model);

    if (setCamera)
        CameraBind(&renderer.camera, shader);
}

void ModelRender(Model *model) {
    for (u32 i = 0; i < model->meshes.size(); i++) {
        Mesh m = model->meshes[i];
        MeshBind(&m);
        glDrawElements(GL_TRIANGLES, m.count, GL_UNSIGNED_INT, nullptr);
    }
}
