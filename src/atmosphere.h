#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "types.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "model.h"

#include <vector>
#include <string>

struct Atmosphere {
    Model dome;
    Shader shader;
};

void AtmosphereCreate(Atmosphere *atmosphere);
void AtmosphereRender(Atmosphere *atmosphere, Camera *camera);

#endif // SKYBOX_H_
