#ifndef __PARSE_ARGS_H_
#define __PARSE_ARGS_H_

#include <stdlib.h>

#include "..\utils\base_log.h"

typedef enum argsFlags 
{
    ARG_InputFile,
    ARG_OutputFile,

    // keep tracks of the numbers of flags suported
    NB_ARGS_FLAGS
} ArgsFlags;

typedef struct arg{
    size_t nb;
    char** args;
} ARG;  

typedef struct args{
    unsigned char allocate[NB_ARGS_FLAGS];
    ARG args[NB_ARGS_FLAGS];
} ARGS;


ARGS argsParser(size_t argSize, char* args[]);

#endif