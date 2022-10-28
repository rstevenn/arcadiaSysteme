#include "utils/std.h"
#include "headers/render_math.h"
#include "headers/camera.h"

#include <string.h>
#include <SDL2/SDL.h>

#define WIDTH 500
#define HEIGHT 500
#define FPS 1000


int main(int argc, char* argv[])
{

    /*
        TODO:

            Texture
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
    char buffer[WIDTH*HEIGHT*4];  
    memset(buffer, 0, WIDTH*HEIGHT*sizeof(Uint32));

    float dist[WIDTH*HEIGHT];  
    memset(dist, 0, WIDTH*HEIGHT*sizeof(float));

    // define camera    
    Camera cam;
    cam.flength = 1000;
    cam.h = HEIGHT;
    cam.w = WIDTH;
    cam.pos = getv3(200, 200, 0);
    cam.angle = getv3(0, 0, 0);

    // define triangle
    triangle3d tri[2];
    tri[0] = get3dtri(getv3(100, 100, 110),
                      getv3(300, 100, 110),
                      getv3(200, 300, 110));

    tri[1] = roatation3dtri(tri[0], getv3(0, 90, 0), getv3(200, 100, 110));

    // render
    for (int i=0; i<5000; i++)
    {
        memset(buffer, 0, WIDTH*HEIGHT*sizeof(Uint32));
        memset(dist, 0, WIDTH*HEIGHT*sizeof(float));
        
        cam.angle.z = cam.angle.z+.5;
        tri[0] = roatation3dtri(tri[0], getv3(0, 1, 0), getv3(200, 100, 110));
        tri[1] = roatation3dtri(tri[1], getv3(0, 1, 0), getv3(200, 100, 110));
        renderTri(cam, buffer, dist, tri, 2);

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
    
    return 0;
}

