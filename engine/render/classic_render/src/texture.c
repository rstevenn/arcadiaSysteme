#include "../headers/texture.h"

// Perlin noise from https://gist.github.com/nowl/828013
static int SEED = 0;

static int hash[] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};

int noise2(int x, int y)
{
    int tmp = hash[(y + SEED) % 256];
    return hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
    return x + s * (y-x);
}

float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

float noise2d(float x, float y)
{
    int x_int = x;
    int y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise2(x_int, y_int);
    int t = noise2(x_int+1, y_int);
    int u = noise2(x_int, y_int+1);
    int v = noise2(x_int+1, y_int+1);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}


// get 2dperlinoise texture
void perlineTexture(char* buffer, int w, int h, v2 pos)
{
    for (int i=0; i<w; i++)
    {
        for (int j=0; j<h; j++)
        {
            int id = (i+j*w)*3;
            float value = noise2d(pos.x+ (float)i/10, pos.y+(float)j/10);

            buffer[id] = (int)(value) %255;
            buffer[id+1] = (int)(value) %255;
            buffer[id+2] = (int)(value) %255;
        }
    }
}

// Textures

TexturedTriangle3d getBaseTextureTriangle(v3 a, v3 b, v3 c)
{
    TexturedTriangle3d tri;
    tri.pos.a = a;
    tri.pos.b = b;
    tri.pos.c = c;

    return tri;
}

// getter
TexturedTriangle3d get3dtriColor(v3 a, v3 b, v3 c, v3 color)
{
    TexturedTriangle3d tri = getBaseTextureTriangle(a, b, c);
    
    tri.data.color = color;
    tri.type = TEXTURE_COLOR;
    tri.getColor = getColorTexture;

    return tri;
}

TexturedTriangle3d get3dtriBaryColor(v3 a, v3 b, v3 c, v3 colora, v3 colorb, v3 colorc)
{
    TexturedTriangle3d tri = getBaseTextureTriangle(a, b, c);
    
    tri.data.bcolor.a = colora;
    tri.data.bcolor.b = colorb;
    tri.data.bcolor.c = colorc;

    tri.type = TEXTURE_BARY_COLOR;
    tri.getColor = getBaryColorTexture;

    return tri;
}

TexturedTriangle3d get3dtriBaryBuffer(v3 a, v3 b, v3 c, char* buffer, triangle2d buffferPos, int width)
{
    TexturedTriangle3d tri = getBaseTextureTriangle(a, b, c);
    
    tri.data.bbuffer.pos = buffferPos;
    tri.data.bbuffer.textBuffer = buffer;
    tri.data.bbuffer.width = width;

    tri.type = TEXTURE_BARY_BUFFER_POS;
    tri.getColor = getBaryBufferTexture;

    return tri;
}


// color
v3 getColorTexture(v3 baryPos, TextureData data)
{
    return data.color;
}

v3 getBaryColorTexture(v3 baryPos, TextureData data)
{
    return getv3(baryblend(baryPos, data.bcolor.a.x, data.bcolor.a.y,  data.bcolor.a.z),
                 baryblend(baryPos, data.bcolor.b.x, data.bcolor.b.y,  data.bcolor.b.z),
                 baryblend(baryPos, data.bcolor.c.x, data.bcolor.c.y,  data.bcolor.c.z));
}

v3 getBaryBufferTexture(v3 baryPos, TextureData data)
{
    v2 pos = getv2(baryblend(baryPos, data.bbuffer.pos.a.x, data.bbuffer.pos.b.x, data.bbuffer.pos.c.x),
                   baryblend(baryPos, data.bbuffer.pos.a.y, data.bbuffer.pos.b.y, data.bbuffer.pos.c.y));

    //INFO("%f %f", pos.x, pos.x)
    int linPos = ((int)pos.x+(int)pos.y*data.bbuffer.width)*3;

    return getv3(data.bbuffer.textBuffer[linPos],
                 data.bbuffer.textBuffer[linPos+1],
                 data.bbuffer.textBuffer[linPos+2]);
}