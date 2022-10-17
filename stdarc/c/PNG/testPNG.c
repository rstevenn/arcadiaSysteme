#define INPUT_PATH "basedLain.png"
#define OUTPUT_PATH "out.PNG"

#include <stdlib.h>
#include <stdio.h>

#include "../utils/headers/base_log.h"
#include "PNG.h"

int main(void)
{
    int buffer[1024*1024];
    //int * buffer = NULL;
    PNG* image = openPNG(INPUT_PATH); 
    readPNG(image, buffer);
    writePNG(image, buffer);
    closePNG(image);

    INFO("finished normally");


    return 1;
}