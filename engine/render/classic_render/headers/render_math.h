#ifndef __RENDER_MATH_H__
#define __RENDER_MATH_H__

// vect
typedef struct v3_
{
    float x;
    float y;
    float z;
} v3;

typedef struct v2_
{
    float x;
    float y;
} v2;

typedef struct triangle_3d
{
    v3 a;
    v3 b;
    v3 c;
} triangle3d;

typedef struct triangle_2d
{
    v2 a;
    v2 b;
    v2 c;
} triangle2d;


// triangles
triangle2d get2dtri(v2 a, v2 b, v2 c);
triangle3d get3dtri(v3 a, v3 b, v3 c);

int inside2dtri(triangle2d t, v2 p);

// barycentric coords
v3 barycentric(v2 a, v2 b, v2 c, v2 p);
float baryblend(v3 coord, float a, float b, float c);

// project to a 2d plane on z=0 
// with a focal length of flength
v2 project2d(v3 point, float flenght);


// rotation
v3 rotation3Dpoint(v3 point, v3 angle, v3 rotor);
v3 rotation3DXpoint(v3 point, float angle);
v3 rotation3DYpoint(v3 point, float angle);
v3 rotation3DZpoint(v3 point, float angle);

triangle3d roatation3dtri(triangle3d tri, v3 angle, v3 rotor);


v2 getv2(float a, float b);
v3 getv3(float a, float b, float c);

// v2 math
v2 v2plusv2(v2 a, v2 b);
v2 v2minusv2(v2 a, v2 b);
v2 v2multv2(v2 a, v2 b);
v2 v2divv2(v2 a, v2 b);

v2 v2plusfloat(v2 a, float b);
v2 v2minusfloat(v2 a, float b);
v2 v2multfloat(v2 a, float b);
v2 v2divfloat(v2 a, float b);

float v2cross(v2 a, v2 b);

// v3 math 
v3 v3plusv3(v3 a, v3 b);
v3 v3minusv3(v3 a, v3 b);
v3 v3multv3(v3 a, v3 b);
v3 v3divv3(v3 a, v3 b);

v3 v3plusfloat(v3 a, float b);
v3 v3minusfloat(v3 a, float b);
v3 v3multfloat(v3 a, float b);
v3 v3divfloat(v3 a, float b);

float dist3d(v3 a, v3 b);

#endif