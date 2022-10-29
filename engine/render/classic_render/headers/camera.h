#include "../headers/render_math.h"
#include "../utils/std.h"
#include "../headers/texture.h"

#ifndef __CAMERA_H__
#define __CAMERA_H__

typedef struct 
{
    float flength;
    int w;
    int h;

    v3 pos;
    v3 angle;

} Camera;

Camera getCamera(v3 pos, v3 angle, float flength, int w, int h);
void renderTri(Camera cam, char* buffer, float* distBuffer, triangle3d* tri, int nbTri);


#endif