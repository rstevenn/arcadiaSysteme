
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../headers/render_math.h"
#include "../utils/std.h"


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
    char* textBuffer; // each pixel start at pos+0 to pos+2  +0 = r,  +1 = g, +2 = b 
    triangle2d pos;
    int width;
} BaryBuffer;

typedef union 
{
    v3 color;
    BaryColor bcolor;
    BaryBuffer bbuffer;   
} TextureData;

typedef struct {
    triangle3d pos;
    TextureType type;
    TextureData data;
    v3(*getColor)(v3 baryPos, TextureData data);
} TexturedTriangle3d;

// geters
void perlineTexture(char* buffer, int w, int h, v2 pos);

TexturedTriangle3d get3dtriColor(v3 a, v3 b, v3 c, v3 color);
TexturedTriangle3d get3dtriBaryColor(v3 a, v3 b, v3 c, v3 colora, v3 colorb, v3 colorc);
TexturedTriangle3d get3dtriBaryBuffer(v3 a, v3 b, v3 c, char* buffer, triangle2d buffferPos, int width);

v3 getColorTexture(v3 baryPos, TextureData data);
v3 getBaryColorTexture(v3 baryPos, TextureData data);
v3 getBaryBufferTexture(v3 baryPos, TextureData data);



#endif