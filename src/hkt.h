#ifndef HKT_H_
#define HKT_H_

#include "types.h"
#include "shader.h"
#include "texture.h"

struct Hkt {
    Texture dYCoeficcientsImage;
    Texture dXCoeficcientsImage;
    Texture dZCoeficcientsImage;

    Texture h0kImage, h0minusKImage;

    u32 N, L;

    Shader shader;
};

void HktCreate(Hkt *hkt, u32 N, u32 L);
void HktRender(Hkt *hkt, f32 t);

#endif // HKT_H_
