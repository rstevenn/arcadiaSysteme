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

int main(int argc, char* argv[])
{

    /*
        TODO:
            Get cam
            Textures
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
    memset(dist, 0, WIDTH*HEIGHT*sizeof(float));

    // define camera    
    Camera cam = getCamera(getv3(200, 200, 0), getv3(0, 0, 0), 1000,
                           WIDTH, HEIGHT);

    // define triangle
    triangle3d tri[2];
    tri[0] = get3dtri(getv3(100, 100, 110),
                      getv3(300, 100, 110),
                      getv3(200, 300, 110));

    tri[1] = roatation3dtri(tri[0], getv3(0, 90, 0), getv3(200, 100, 110));

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
        tri[0] = roatation3dtri(tri[0], getv3(0, .5, 0), getv3(200, 100, 110));
        tri[1] = roatation3dtri(tri[1], getv3(0, .5, 0), getv3(200, 100, 110));


        // render
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

    INFO("END NORMALY")
    
    return 0;
}

