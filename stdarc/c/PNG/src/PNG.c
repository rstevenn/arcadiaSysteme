#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/headers/base_log.h"
#include "../utils/headers/nb_convert.h"

#include "../headers/PNG.h"


// ###########
// open close
// ###########

PNG* openPNG(char* path)
{
    PNG* img = (PNG*)malloc(sizeof(PNG));
    CHECK_ALLOCATE(img, "Unable to allocate PNG descriptor")

    img->fp = fopen(path, "rwb");
    CHECK_ALLOCATE(img->fp, "Unable to open the file %s", path)

    // header data init
    img->hasPLTE = 0;

    img->rawData = NULL;
    img->rawDataSize = 0;

    return img;
}

int closePNG(PNG* img)
{
    fclose(img->fp);
    free(img);

    return 1;
}


// ###########
// read write
// ###########

int readPNG(PNG* img, int* buffer)
{
    // main var
    char* rawData = NULL;
    size_t rawDataSize = 0;

    // check 8bits PNG header
    char PNGheader[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    char FILEHEADER[8];

    CHECK_READ_WRITE(fread(FILEHEADER, sizeof(char), 8, img->fp), 8, "Unable to read the PNG 8bits header")

    for (size_t i=0; i<8; i++)
    {
        if (PNGheader[i] != FILEHEADER[i])
            ERROR("Invalid PNG HEADER")
    }

    // read chunks
    int next = 1;
    int chunkId = 0;

    char lbuffer[4];
    unsigned int length;

    char tbuffer[5] = {0, 0, 0, 0, '\0'};
    unsigned int typeFlag;
    unsigned int typeResume = 0;

    char* dbuffer;

    char cbuffer[4];

    while (next) {
        // read data length
        CHECK_READ_WRITE(fread(lbuffer, sizeof(char), 4, img->fp), 4, "Unable to read the chunk %d length", chunkId)
        length = big_endian(lbuffer);

        // read type
        CHECK_READ_WRITE(fread(tbuffer, sizeof(char), 4, img->fp), 4, "Unable to read the chunk %d type", chunkId)
        typeFlag = type2Flag(tbuffer);
        
        validate_type(typeFlag, typeResume);
        typeResume  |= typeFlag;

        // read data
        dbuffer = (char*)malloc(sizeof(char)*length);

        CHECK_ALLOCATE(dbuffer, "Unable to allocate the bufffer to read data")
        CHECK_READ_WRITE(fread(dbuffer, sizeof(char), length, img->fp), length, "Unable to read the chunk %d data", chunkId)

        extract_data(img, typeFlag, dbuffer, length);

        free(dbuffer);

        // read crc
        CHECK_READ_WRITE(fread(cbuffer, sizeof(char), 4, img->fp), 4, "Unable to read the chunk %d crc", chunkId)
        validate_crc(cbuffer);
        

        chunkId++;
        next = !(typeFlag == IEND);    
    }

    return 1;
}

int writePNG(PNG* img, int* buffer)
{


    NOT_IMPLEMENTED()
    return 1;
}

// #####################
// chunk type handeling
// #####################

unsigned int type2Flag(char strtype[5])
{
    if (!strcmp("IHDR", strtype)) {
        return IHDR;
    } 
    else if (!strcmp("PLTE", strtype)) {
        return PLTE;
    } 
    else if (!strcmp("IDAT", strtype)) {
        return IDAT;
    } 
    else if (!strcmp("IEND", strtype)) {
        return IEND;
    } 
    else if (!strcmp("cHRM", strtype)) {
        return cHRM;
    } 
    else if (!strcmp("gAMA", strtype)) {
        return gAMA;
    } 
    else if (!strcmp("iCCP", strtype)) {
        return iCCP;
    } 
    else if (!strcmp("sBIT", strtype)) {
        return sBIT;
    } 
    else if (!strcmp("sRGB", strtype)) {
        return sRGB;
    } 
    else if (!strcmp("bKGD", strtype)) {
        return bKGD;
    } 
    else if (!strcmp("hIST", strtype)) {
        return hIST;
    } 
    else if (!strcmp("tRNS", strtype)) {
        return tRNS;
    } 
    else if (!strcmp("pHYs", strtype)) {
        return pHYs;
    } 
    else if (!strcmp("sPLT", strtype)) {
        return sPLT;
    } 
    else if (!strcmp("tIME", strtype)) {
        return tIME;
    } 
    else if (!strcmp("iTXt", strtype)) {
        return iTXt;
    } 
    else if (!strcmp("tEXt", strtype)) {
        return tEXt;
    } 
    else if (!strcmp("zTXt", strtype)) {
        return zTXt;
    } else {
        ERROR("Unknow type \"%s\"", strtype)
    }
}

// #######
// checks
// #######

void validate_type(unsigned int currentType, unsigned int typesResume)
{
    // header in 1st
    if (currentType == IHDR) 
    {
        if (typesResume != 0)
            ERROR("multiple headers blocks")
        return;
    }

    if (typesResume == 0)
        ERROR("the 1st block isn't a header block")

    // PLTE block
    if (currentType == PLTE)
    {
        if (PLTE&typesResume)
            ERROR("multiple Palette blocks")

        

        if ( (typesResume&tRNS) || (typesResume&hIST) || (typesResume&bKGD))
            ERROR("Palette block is before iDAT tRNS hIST or bKGD blocks")
        return;
    
    }

    // IDAT block
    if (currentType == IDAT)
    {
        // check that PLTE is before (tRNS hIST bKGD)
        return;
    }

    // IEND block
    if (currentType == IEND)
        return;

    NOT_IMPLEMENTED()

}

void validate_crc(char* data)
{
    NOT_IMPLEMENTED()
}

// #############
// extract data
// #############
void extract_data(PNG* img, unsigned int type, char* data, size_t length)
{
    if (type == IHDR)
    {
        if (length != 13)
            ERROR("invalid header length %d!=13 (real!=expected)", (int)length)

        // width / heigth
        img->width = big_endian(data);
        img->height = big_endian((data+4));

        // check img size is not 0
        if (!(img->width|img->height))
            ERROR("invalid image size (width and height must be non zero)")


        // bit depth
        img->bitDepth = (unsigned char)data[8];

        // color type
        img->colorType = (unsigned char)data[9];

        // check the color type and bit depth
        unsigned int cd = img->bitDepth;

        switch (img->colorType) 
        {
            case GREYSCALE: 
                if (!(cd == 1 || cd == 2 || cd == 4 || cd == 8 || cd == 16))
                    ERROR("Invalid bitDepth %u for greyscale image", cd);
                break;
            
            case TRUECOLOUR: 
                if (!(cd == 8 || cd == 16))
                    ERROR("Invalid bitDepth %u for true colour image", cd);
                break;
            
            case INDEXED_COLOUR: 
                if (!(cd == 1 || cd == 2 || cd == 4 || cd == 8))
                    ERROR("Invalid bitDepth %u for indexed colour image", cd);
                break;

            case GREYSCALE_ALPHA: 
                if (!(cd == 8 || cd == 16))
                    ERROR("Invalid bitDepth %u for greyscale with alpha image", cd);
                break;

            case TRUECOLOUR_ALPHA: 
                if (!(cd == 8 || cd == 16))
                    ERROR("Invalid bitDepth %u for truecolour with alpha image", cd);
                break;

            default:
                ERROR("Invalid colortype %u", img->colorType)
        }

        // extract compression method, filter method and interlace method
        img->compression = (unsigned char)data[10];
        img->filter = (unsigned char)data[11];
        img->interlace = (unsigned char)data[12];

        // show
         show_header_data(img);

    } else if (type == PLTE) {
        // check length
        if (length%3 != 0)
            ERROR("Palette length must be a multible of 3")

        if (length<3 || length > 3*256)
            ERROR("Palette length must be between 1 and 256")

        // check valid image type for palette
        if (!(img->colorType==TRUECOLOUR || img->colorType==INDEXED_COLOUR  || img->colorType==TRUECOLOUR_ALPHA))
            ERROR("The current image type [%d] doesn't support color palette", img->colorType)

        // check that length < 2**bitdepth
        if ((1<<img->bitDepth) >= length/3)
            ERROR("The current length of palette is greater than 2*bitdepth")

        // copy data
        memcpy(img->PLTE, data, length);
        
        img->PLTElength = length;
        img->hasPLTE = 1;

    } else if (type == IDAT){
        // check if PLTE is present for index couloured image
        if (img->colorType == INDEXED_COLOUR && !(img->hasPLTE)) 
            ERROR("index coloured images must have a palette")

        img->rawDataSize += length;
        img->rawData = realloc(img->rawData, img->rawDataSize);
        CHECK_ALLOCATE(img->rawData, "unable to allocate memory of size [%d bits] for the data", img->rawData);

    } else if (type == IEND){
        if (length > 0)
            ERROR("the iend block should not contain data")

    } else {
        NOT_IMPLEMENTED()
    }
}

void show_header_data(PNG* img){
    printf("size (width x height): %upx %upx\n", img->width, img->height);
    printf("color type:            ");

    switch (img->colorType) 
    {
        case GREYSCALE:
            printf("greyscale\n");
            break;
        
        case TRUECOLOUR:
            printf("true colour\n");
            break;

        case INDEXED_COLOUR:
            printf("indexed colour\n");
            break;

        case GREYSCALE_ALPHA:
            printf("greyscale with alpha\n");
            break;

        case TRUECOLOUR_ALPHA: 
            printf("truecolour with alpha\n");
            break;

        default:
            break;
    }

    printf("compression method:    %u\n", img->compression);
    printf("Filter method:         %u\n", img->filter);
    printf("Interlace method:      %u\n", img->interlace);    
}