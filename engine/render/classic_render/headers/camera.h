#include "../headers/render_math.h"
#include "../utils/std.h"

#ifndef __CAMERA_H__
#define __CAMERA_H__

typedef struct 
{
    float flength;
    int w;
    int h;

} Camera;


void renderTri(Camera cam, char* buffer, float* distBuffer, triangle3d* tri, int nbTri);


#endif