#include "../headers/render_math.h"
#include "../utils/std.h"

#include <math.h>

v3 barycentric(v2 a, v2 b, v2 c, v2 p)
{
    v2 v_0 = v2minusv2(b, a);
    v2 v_1 = v2minusv2(c, a);
    v2 v_2 = v2minusv2(p, a);

    float den = v2cross(v_0, v_1);

    v3 result;
    result.y = v2cross(v_2, v_1) / den;
    result.z = v2cross(v_0, v_2) / den;
    result.x = 1 - result.z - result.y;

    return result;
}

float baryblend(v3 coord, float a, float b, float c)
{
    return coord.x*a + coord.y*b + coord.z*c;
}


v2 project2d(v3 point, float flenght)
{
    v2 result;
    result.x = (flenght*point.x) / (flenght+point.z);
    result.y = (flenght*point.y) / (flenght+point.z);

    return result;
}



// triangle

triangle2d get2dtri(v2 a, v2 b, v2 c)
{
    triangle2d t;
    t.a = a;
    t.b = b;
    t.c = c;

    return t;
}


triangle3d get3dtri(v3 a, v3 b, v3 c)
{
    triangle3d t;
    t.a = a;
    t.b = b;
    t.c = c;

    return t;
}



float sign (v2 p1, v2 p2, v2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

int inside2dtri(triangle2d t, v2 p)
{
    v3 coord = barycentric(t.a, t.b, t.c, p);

    return (0 <= coord.x && coord.x <= 1) &&
           (0 <= coord.y && coord.y <= 1) &&
           (0 <= coord.z && coord.z <= 1);
}


// rotation
triangle3d roatation3dtri(triangle3d tri, v3 angle, v3 rotor)
{
    return get3dtri(rotation3Dpoint(tri.a, angle, rotor),
                    rotation3Dpoint(tri.b, angle, rotor),
                    rotation3Dpoint(tri.c, angle, rotor));
}

v3 rotation3Dpoint(v3 point, v3 angle, v3 rotor)
{
    v3 localPos = v3minusv3(point, rotor);

    localPos = rotation3DZpoint(localPos, angle.z* (3.1415/180));
    localPos = rotation3DYpoint(localPos, angle.y* (3.1415/180));
    localPos = rotation3DXpoint(localPos, angle.x* (3.1415/180));


    return v3plusv3(localPos, rotor);
}

v3 rotation3DXpoint(v3 point, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return getv3(point.x, c*point.y - s*point.z,
                          s*point.y + c*point.z); 
}

v3 rotation3DYpoint(v3 point, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return getv3(c*point.x + s*point.z, point.y,
                -s*point.x + c*point.z); 
}


v3 rotation3DZpoint(v3 point, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    return getv3(c*point.x - s*point.y,
                 s*point.x + c*point.y, point.z); 
}



// vecotrs 
v2 getv2(float a, float b)
{
    v2 vect;
    vect.x = a;
    vect.y = b;

    return vect;
}

v3 getv3(float a, float b, float c)
{
    v3 vect;
    vect.x = a;
    vect.y = b;
    vect.z = c;

    return vect;
}



// v2 math
v2 v2plusv2(v2 a, v2 b) {
    v2 c;
    c.x = a.x+b.x;
    c.y = a.y+b.y;

    return c;
}

v2 v2minusv2(v2 a, v2 b) {
    v2 c;
    c.x = a.x-b.x;
    c.y = a.y-b.y;

    return c;  
}

v2 v2multv2(v2 a, v2 b) {
    v2 c;
    c.x = a.x*b.x;
    c.y = a.y*b.y;

    return c;  
}

v2 v2divv2(v2 a, v2 b) {
    v2 c;
    c.x = a.x/b.x;
    c.y = a.y/b.y;

    return c;  
}

v2 v2plusfloat(v2 a, float b)
{
    v2 c;
    c.x = a.x+b;
    c.y = a.y+b;

    return c;
}

v2 v2minusfloat(v2 a, float b)
{
    v2 c;
    c.x = a.x-b;
    c.y = a.y-b;

    return c;
}

v2 v2multfloat(v2 a, float b)
{
    v2 c;
    c.x = a.x*b;
    c.y = a.y*b;

    return c;
}

v2 v2divfloat(v2 a, float b)
{
    v2 c;
    c.x = a.x/b;
    c.y = a.y/b;

    return c;
}

float v2cross(v2 a, v2 b)
{
    return a.x*b.y - a.y*b.x;
}

// v3 math 
v3 v3plusv3(v3 a, v3 b)
{
    v3 c;
    c.x = a.x+b.x;
    c.y = a.y+b.y;
    c.z = a.z+b.z;

    return c;
}

v3 v3minusv3(v3 a, v3 b)
{
    v3 c;
    c.x = a.x-b.x;
    c.y = a.y-b.y;
    c.z = a.z-b.z;

    return c;
}

v3 v3multv3(v3 a, v3 b)
{
    v3 c;
    c.x = a.x*b.x;
    c.y = a.y*b.y;
    c.z = a.z*b.z;

    return c;
}

v3 v3divv3(v3 a, v3 b)
{
    v3 c;
    c.x = a.x/b.x;
    c.y = a.y/b.y;
    c.z = a.z/b.z;

    return c;
}

v3 v3plusfloat(v3 a, float b)
{
    v3 c;
    c.x = a.x+b;
    c.y = a.y+b;
    c.z = a.z+b;

    return c;
}

v3 v3minusfloat(v3 a, float b)
{
    v3 c;
    c.x = a.x-b;
    c.y = a.y-b;
    c.z = a.z-b;

    return c;
}

v3 v3multfloat(v3 a, float b)
{
    v3 c;
    c.x = a.x*b;
    c.y = a.y*b;
    c.z = a.z*b;

    return c;
}

v3 v3divfloat(v3 a, float b)
{
    v3 c;
    c.x = a.x/b;
    c.y = a.y/b;
    c.z = a.z/b;

    return c;
}

float dist3d(v3 a, v3 b)
{
    float da = a.x-b.x;
    float db = a.y-b.y;
    float dc = a.z-b.z;
    
    return sqrtf(da*da + db*db + dc*dc  );
}