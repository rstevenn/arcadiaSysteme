#include "utils/std.h"
#include "headers/render_math.h"
#include "headers/camera.h"
#include "headers/texture.h"

#include <string.h>
#include <SDL2/SDL.h>


#define HEIGHT 1000
#define WIDTH (int)(HEIGHT*16/9)
#define FPS 1000

char buffer[WIDTH*HEIGHT*4];
float dist[WIDTH*HEIGHT];    
char noiseTexture[100*100*3];


int main(int argc, char* argv[])
{

    /*
        TODO:
            Textures
            PerlinLikeNoise
    */
   NOT_IMPLEMENTED()

    // INIT
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        ERROR("SDL can't be init")

    // windows
    SDL_Window* wind = SDL_CreateWindow("Frame",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WIDTH, HEIGHT, 0);

    if (!wind)
        ERROR("SDL can't init window")

    // renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, render_flags);

    if (!rend)
        ERROR("SDL can't init renderer")

    // create a texture
    SDL_Texture * texture = SDL_CreateTexture(rend,
                    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);



    // try something
    memset(buffer, 0, WIDTH*HEIGHT*sizeof(Uint32));
    memset(dist,   0, WIDTH*HEIGHT*sizeof(float));

    // define camera    
    Camera cam = getCamera(getv3(200, 200, 0), getv3(0, 0, 0), 1000,
                           WIDTH, HEIGHT);

    // define triangle
    TexturedTriangle3d tri[2];

    v2 perlinPos = getv2(0, 0);
    perlineTexture(noiseTexture, 100, 100, perlinPos);

    triangle3d pos0 = get3dtri(getv3(100, 100, 110),
                               getv3(300, 100, 110),
                               getv3(200, 300, 110));

    tri[0] = get3dtriBaryBuffer(pos0.a, pos0.b, pos0.c, 
                                noiseTexture, get2dtri(getv2(1, 1), getv2(99, 1), getv2(1, 99)),
                                100);


    triangle3d pos1 = roatation3dtri(pos0, getv3(0, 90, 0), getv3(200, 100, 110));
    tri[1] = get3dtriBaryColor(pos1.a,           pos1.b,           pos1.c, 
                               getv3(255, 0, 0), getv3(0, 255, 0), getv3(0, 0, 255));


    // loop
    SDL_Event event;
    int run = 1;
    while (run)
    {
        /* Process events */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    run = 0;
                    break;

                default:
                    break;
            }
        }

        // rotate
        cam.angle.z = cam.angle.z+.1;
        tri[0].pos = roatation3dtri(tri[0].pos, getv3(0, .5, 0), getv3(200, 100, 110));
        tri[1].pos = roatation3dtri(tri[1].pos, getv3(0, .5, 0), getv3(200, 100, 110));


        // render
        renderTri(cam, buffer, dist, tri, 2);

        // write the noise on the top left corner
        for (int k=0; k<100; k++)
        {
            for (int l=0; l<100; l++)
            {
                int id = (k+l*WIDTH)*4;

                buffer[id+3] =   255;
                buffer[id+0] = noiseTexture[(k+l*100)*3];
                buffer[id+1] = noiseTexture[1 + (k+l*100)*3];
                buffer[id+2] = noiseTexture[2 + (k+l*100)*3];
            }
        }

        SDL_RenderClear(rend);
	
        SDL_UpdateTexture(texture, NULL, buffer, WIDTH*sizeof(Uint32));
        SDL_RenderCopy(rend, texture, NULL, NULL);
        SDL_RenderPresent(rend);


        SDL_Delay(1);

    }

    // free resources
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);    
    SDL_Quit();

    INFO("END NORMALY")
    
    return 0;
}

