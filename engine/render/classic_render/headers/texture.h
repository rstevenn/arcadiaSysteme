#include "../headers/render_math.h"
#include "../utils/std.h"

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

typedef enum
{
    TEXTURE_COLOR,
    TEXTURE_BARY_COLOR,
    TEXTURE_BARY_BUFFER_POS
} TextureType;

typedef struct {
    v3 a;
    v3 b;
    v3 c;
} BaryColor;

typedef struct {
    char* textBuffer; // each pixel start at pos+0 to pos+2  +0 = r +1 = g +2 = b 
    triangle2d pos;
    int width;
} BaryBuffer;

typedef union 
{
    char color[4];
    BaryColor bcolor;
    BaryBuffer bbuffer;   
} TextureData;

typedef struct {
    triangle3d pos;
    TextureType type;
    TextureData data;
    v3(*getColor)(v3 baryPos);
} TexturedTriangle3d;



#endif