#ifndef __CRC_32_H__
#define __CRC_32_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define  POLYNOMIAL 0x04C11DB7


typedef uint32_t crc;
crc crcTable[256];

void crcInit(void);
crc crcFast(uint8_t const message[], int nBytes);



#endif