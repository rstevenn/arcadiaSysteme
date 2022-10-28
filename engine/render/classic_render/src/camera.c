#include "../headers/camera.h"
#include <math.h>
#include <stdlib.h>

int getDPos(int x, int y, int w, int h)
{
    return x+y*w;
}

int getIPos(int x, int y, int w, int h)
{
    return (x+(h-y-1)*w)*4;
}

void renderTri(Camera cam, char* buffer, float* distBuffer, triangle3d* tri, int nbTri)
{
    // rendering loop
    for (size_t i=0; i<nbTri; i++)
    {
        // cam pos
        triangle3d localTri = roatation3dtri(tri[i], cam.angle, cam.pos);
        localTri.a = v3minusv3(localTri.a, cam.pos); 
        localTri.b = v3minusv3(localTri.b, cam.pos);
        localTri.c = v3minusv3(localTri.c, cam.pos);


        // project
        v2 a = project2d(localTri.a, cam.flength);
        v2 b = project2d(localTri.b, cam.flength);
        v2 c = project2d(localTri.c, cam.flength);
        triangle2d t = get2dtri(a, b, c);

        // dist
        float da = localTri.a.z;
        float db = localTri.b.z;
        float dc = localTri.c.z;

        // find bounding box
        int x_min = (int)fmin(fmin(a.x, b.x), c.x); 
        int x_max = (int)fmax(fmax(a.x, b.x), c.x);

        int y_min = (int)fmin(fmin(a.y, b.y), c.y);
        int y_max = (int)fmax(fmax(a.y, b.y), c.y);

        float dist;
        v3 bariCoord;
        int i_pos, d_pos;

        // found pixells in triangle
        for (int x=x_min; x<=x_max; x++) {
            for (int y=y_min; y<=y_max; y++) {

                // check in buffer
                if (inside2dtri(t, getv2(x, y))  &&
                    -cam.w/2 <= x && x < cam.w/2 &&
                    -cam.h/2 <= y && y < cam.h/2)
                {
                    bariCoord = barycentric(a, b, c, getv2(x, y));
                    
                    // check dist                    
                    dist = baryblend(bariCoord, da, db, dc);
                    d_pos = getDPos(x+cam.w/2, y+cam.h/2, cam.w, cam.h);                    

                    if (dist > 0 && (dist <= distBuffer[d_pos] || distBuffer[d_pos] == 0)) {
                        
                        distBuffer[d_pos] = dist;

                        // draw color
                        // a = 3, r = 2, g = 1, b = 0 
                        i_pos = getIPos(x+cam.w/2, y+cam.h/2, cam.w, cam.h);
                        buffer[i_pos+3] = 255;
                        
                        buffer[i_pos+2] = (int)baryblend(bariCoord, 255, 0, 0);
                        buffer[i_pos+1] = (int)baryblend(bariCoord, 0, 255, 0);
                        buffer[i_pos+0] = (int)baryblend(bariCoord, 0, 0, 255); 
                    }
                }
            }
        }
    }
}
