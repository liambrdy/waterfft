#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "types.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"

#include <vector>
#include <string>

struct Skybox {
    u32 texture;
    Mesh box;
    Shader shader;
};

void SkyboxCreate(Skybox *skybox, const std::vector<std::string> &paths);
void SkyboxRender(Skybox *skybox, Camera *camera);

#endif // SKYBOX_H_
