#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wired.def.h"
#include "utils/base_log.h"

char* readAllFile(char* path, size_t* file_size_out)
{
    // open file
    FILE* fp = fopen(path, "rb");
    CHECK_ALLOCATE(fp, "Can't read the file %s", path);

    // get file size
    fseek(fp, 0, SEEK_END); 
    size_t size = ftell(fp); 
    fseek(fp, 0, SEEK_SET);
    *file_size_out = size;

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
    // check args
    if (argc <= 1)
        ERROR("No file pass as arg")

    // read file
    size_t file_size;
    char* rawText = readAllFile(argv[1], &file_size);
    char* current = rawText;
    INFO("RAED : '%s' %lu bits", argv[1], file_size)

    // extract and sanitize header metadata
    if (file_size < sizeof(wired_vm_header_t))
        ERROR("the file is to short to contain a header")
    
    wired_vm_header_t header = *(wired_vm_header_t*)current;
    current += sizeof(header);
    size_t pgm_size = file_size - (current - rawText);

    if (strcmp("WIRE", header.identificator) != 0)
        ERROR("Invalid indentificator")
    
    if (header.ram_size < pgm_size + header.stack_size)
        ERROR("Not enough ram allocated")

    if (header.entry_point > pgm_size)
        ERROR("Entry point out of executable zone");

    // setup ram
    INFO("init ram of %lu bits and load program", header.ram_size);
    char* vm_ram = (char*)malloc(header.ram_size*sizeof(char));
    if (vm_ram == NULL)
        ERROR("Can't allocate ram memory")

    if (memcpy(vm_ram, current, pgm_size*sizeof(char)) == NULL) 
        ERROR("Can't copy program on ram")

    // setup registers
    INFO("setup registers")
    uint64_t registers[REGISTERS_NB] = {0};
    registers[SP] = header.ram_size - 1 - header.stack_size;
    registers[PC] = header.entry_point;


    // clear
    free(vm_ram);
    free(rawText);
    INFO("END")
    return 0;
}