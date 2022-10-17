#ifndef __PNG__
#define __PNG__


typedef struct png_descriptor
{
    unsigned int width;
    unsigned int height;

    char bitDepth;
    char colorType;
    char compression;
    char filter;
    char interlace;

    int hasPLTE : 1;
    char PLTE[256*3];
    char PLTElength;

    char* rawData;
    size_t rawDataSize;

    FILE* fp;
} PNG;


enum ColorType {
    GREYSCALE = 0,
    TRUECOLOUR = 2,
    INDEXED_COLOUR = 3,
    GREYSCALE_ALPHA = 4,
    TRUECOLOUR_ALPHA = 6
};

enum ChunkType {
    IHDR = 1,
    PLTE = 1<<1,
    IDAT = 1<<2,

    cHRM = 1<<3,
    gAMA = 1<<4,
    iCCP = 1<<5,
    sBIT = 1<<6,
    sRGB = 1<<7,
    bKGD = 1<<8,
    hIST = 1<<9,
    tRNS = 1<<10,
    pHYs = 1<<11,
    sPLT = 1<<12,
    tIME = 1<<13,
    iTXt = 1<<14,
    tEXt = 1<<15,
    zTXt = 1<<16,
    IEND = 1<<17
};

// chunk type handeling
unsigned int type2Flag(char strtype[5]);

// basic handling
PNG* openPNG(char* path);
int closePNG(PNG* img);

// read / write
int readPNG(PNG* img, int* buffer);
int writePNG(PNG* img, int* buffer);

// checks
void validate_type(unsigned int currentType, unsigned int typesResume);
void validate_crc(char* data);

// extract data
void extract_data(PNG* img, unsigned int type, char* data, size_t length);
void show_header_data(PNG* img);

#endif