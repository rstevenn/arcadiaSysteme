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
            v3 math

            3d point rotation
            3d triangle rotation

            camera pos
            camera angle
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
    cam.flength = 100;
    cam.h = HEIGHT;
    cam.w = WIDTH;

    // define triangle
    triangle3d tri[2];
    tri[0] = get3dtri(getv3(100, 100, 100),
                      getv3(300, 100, 120),
                      getv3(200, 300, 110));

    tri[1] = get3dtri(getv3(150, 100, 110),
                      getv3(350, 100, 110),
                      getv3(250, 300, 110));

    // render
    renderTri(cam, buffer, dist, tri, 2);
    INFO("END")

    SDL_RenderClear(rend);
	
    SDL_UpdateTexture(texture, NULL, buffer, WIDTH*sizeof(Uint32));
    SDL_RenderCopy(rend, texture, NULL, NULL);
    SDL_RenderPresent(rend);


    SDL_Delay(20000);



    // free resources
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);    
    SDL_Quit();
    
    return 0;
}

