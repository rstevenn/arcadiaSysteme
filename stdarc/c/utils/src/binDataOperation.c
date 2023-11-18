#include "../headers/binDataOperation.h"
#include <string.h>

/*
    STRUCT FOR COMPRESSION
*/

typedef struct {
    int data;
    int nb;
} BlockCount;

typedef struct {
    char* data;
    size_t size;
    size_t blockSize;
} Blocks;

typedef struct compress_Node {
    int terminal;
    int value;
    int frequence; 
    
    int left;
    int right;
} CompressNode;

typedef struct compress_tree {
    CompressNode* nodes;
    size_t size;
} CompressTree;

typedef struct compress_header{
    size_t blockSize;
    size_t blockLength;
    size_t treeLeafLength;
    size_t compressedLength;
} CompressedHeader;


/*
    COMPRESSION
*/

int compressHelperDataBlockEqual(char* a, char* b, size_t blockSize) {
    
    for (size_t i=0; i<blockSize; i++){
        if (a[i] != b[i])
            return 0;
    }
    
    return 1;
}

void compress(char* data, char* compressed, size_t length, size_t* cLength, size_t blockSize, size_t steps)
{
    if (steps == 0) {
        compressed = memccpy(compressed, data, sizeof(char), length);
        CHECK_ALLOCATE(compressed, "Unable to copy data")
        cLength[0] = length; 

        return;    
    }

    // setup book of blocks
    Blocks book;
    book.blockSize = blockSize;
    book.size = 0;
    book.data = NULL;

    // count data block 
    BlockCount* blockCount = NULL;
    size_t blockCountNb = 0;

    if (length%blockSize != 0)
        ERROR("Length [%u] must be a multiple of block size [%u]", length, blockSize)

    for (size_t i=0; i<length/blockSize; i++) {
        int found = 0;
     
        for (size_t j=0; j<blockCountNb; j++) {
            if (compressHelperDataBlockEqual(book.data[blockCount[j].data], &(data[i*blockSize]), blockSize)) {
                found = 1;
                break;
            }
        }

        if (!found) {
            
        }
    }


    // sort data block
    

}


void deCompress(char* data, char* compressed, size_t* length, size_t cLength, size_t blockSize, size_t steps)
{
    if (steps == 0) {
        return;    
    }

}