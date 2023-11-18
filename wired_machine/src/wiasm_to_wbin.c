#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __HASH_MAP_IMPL__
#include "utils/hash_map.h"
#include "utils/base_log.h"
#include "wired.def.h"



char* readAllFile(char* path)
{
    // open file
    FILE* fp = fopen(path, "rb");
    CHECK_ALLOCATE(fp, "Can't read the file %s", path);

    // get file size
    fseek(fp, 0, SEEK_END); 
    size_t size = ftell(fp); 
    fseek(fp, 0, SEEK_SET);

    // read data
    char* buffer = (char*) malloc(sizeof(char) * (size+1));
    CHECK_ALLOCATE(buffer, "Unable to allocate a buffer of %lu chars", size)

    size_t got;
    CHECK_READ_WRITE(size, got=fread(buffer, sizeof(char), size, fp), "unable to read the file %s (expected %lu != got %lu)", path, size, got);
    buffer[got] = '\0';

    // close file
    fclose(fp);

    return buffer;
} 


int main(size_t argc, char* argv[])
{

    INFO("%lu", sizeof(operation_t)*8)
    return 0;

    // check args
    if (argc <= 1)
        ERROR("No file pass as arg")

    // read file
    char* rawText = readAllFile(argv[1]);
    INFO("raw txt:\n\n%s\n", rawText);
    
    wired_vm_header_t header;

    // stack and ram size
    size_t ok;
    ok = sscanf(rawText, "ram_size:%lu\n", &(header.ram_size));
    if (!ok) {
        ERROR("Invalid ram size or ram size not privided")
    }

    char* next = strpbrk(rawText, "\n");
    if (next == NULL) {
        ERROR("Stack not privided")
    } 

    ok = sscanf(next+1, "stack_size:%lu\n", &(header.stack_size));
    if (!ok) {
        ERROR("Invalid stack size or ram size not privided")
    }

    next = strpbrk(next+1, "\n");
    if (next == NULL) {
        ERROR("no code found")
    } 
    next++;

    // instructions and build label table
    char buffer_base[64] = {0};
    char* buffer_current = buffer_base;

    // translater state

    char in_instruction = 0;
    size_t nb_args = 0;
    long int instruction_id = 0;

    for (char* current = next; (*current) != '\0'; current++) {
		
        switch (*current) {
            
            // split chars
            case ' ': 
            case '\t': 

                buffer_current = buffer_base;
                if (!in_instruction) {
                    // parse instruction buffer
                    in_instruction = 1;
                    nb_args = 1;
                } else {
                    // parse args
                    nb_args++;
                }

                break;
            

            case '\n': {
                if (in_instruction) {
                    // parse_args
                    nb_args++;
                }

                buffer_current = buffer_base;
                in_instruction = 0;
                break;
            }

            // lable
            case ':':
                buffer_current = '\0';
                table_add_label(buffer_base);
                buffer_current = buffer_base;

                break;

            default:
                if (buffer_current - buffer_base > 63) 
                    ERROR("Token bigger than 64 bits")
                
                *buffer_current = *current;
                buffer_current++;
        }
        
        *buffer_current = '\0';
        INFO("current = '%c', buffer = '%s'", *current, buffer_base)
    }

    // set label


    INFO("ENDED")
    return 1;
}
