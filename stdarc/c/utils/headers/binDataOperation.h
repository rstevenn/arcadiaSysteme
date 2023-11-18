#ifndef __BINDOP_H__
#define __BINDOP_H__

#include "base_log.h"


void compress(char* data, char* compressed, size_t length, size_t* cLength, size_t blockSize, size_t steps);
void deCompress(char* data, char* compressed, size_t* length, size_t cLength, size_t blockSize, size_t steps);


#endif