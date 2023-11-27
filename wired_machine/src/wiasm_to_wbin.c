#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __HASH_MAP_IMPL__
#include "utils/hash_map.h"
#include "utils/base_log.h"

#include "wired.def.h"
#include "parse_wiasm.h"


void render_pgm(operation_t* op, size_t* size_out, char* data_out){

    switch (op->size)
    {
    case inst_128: {
        raw_128_op render_op;
        render_op.op_code = op->type;
        render_op.args.arg0 = op->args.arg_64.arg0;

        *size_out = sizeof(raw_128_op);
        CHECK_ALLOCATE(memcpy(data_out, &render_op, sizeof(raw_128_op)), "can't copy instruction data");
        break;
    }

    case inst_192: {
        raw_192_op render_op;
        render_op.op_code = op->type;
        render_op.args.arg0 = op->args.arg_128.arg0;
        render_op.args.arg1 = op->args.arg_128.arg1;

        *size_out = sizeof(raw_192_op);
        CHECK_ALLOCATE(memcpy(data_out, &render_op, sizeof(raw_192_op)), "can't copy instruction data");
        break;
    }

    case inst_256: {
        raw_256_op render_op;
        render_op.op_code = op->type;
        render_op.args.arg0 = op->args.arg_192.arg0;
        render_op.args.arg1 = op->args.arg_192.arg1;
        render_op.args.arg2 = op->args.arg_192.arg2;

        *size_out = sizeof(raw_256_op);
        CHECK_ALLOCATE(memcpy(data_out, &render_op, sizeof(raw_256_op)), "can't copy instruction data");
        break;
    }

    default:
        ERROR("Unknow instruction with type %lu", op->size)
        break;
    }
}


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
    // check args
    if (argc <= 1)
        ERROR("No file pass as arg")

    // read file
    char* rawText = readAllFile(argv[1]);
    INFO("raw txt:\n\n%s\n", rawText);
    
    wired_vm_header_t header = {0};

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
    long int instruction_addr = 0;

    program_t program = {0};
    
    char* current = next;
    char found_entry = 0;
    int i = 0;
    
    while(current != NULL){

        //INFO("%d", i)
        i++;

        switch (*current) {
            
            // split chars
            case ' ': 
            case '\t':
            case '\r': 
            case '\0': 
                if (buffer_base == buffer_current)
                    break;

                buffer_current = buffer_base;
                if (!in_instruction) {
                    instruction_addr += parse_instruction(buffer_base, &program);
                    in_instruction = 1;
                    nb_args = 0;
                
                } else {
                    (*program.operations[program.len-1].parse_args)(buffer_base, nb_args, &program.operations[program.len-1]);
                    nb_args++;
                }

                break;
            

            case '\n': {
                if (buffer_base == buffer_current && !in_instruction)
                    break;

                if (!in_instruction) {
                    instruction_addr += parse_instruction(buffer_base, &program);
                }

                buffer_current = buffer_base;
                in_instruction = 0;
                nb_args = 0;
                break;
            }

            // lable
            case ':':
                buffer_current = '\0';
                table_add_label(buffer_base);
                table_add_adrr(buffer_base, instruction_addr);
                buffer_current = buffer_base;

                // add entry point
                if (strcmp(buffer_base, "ENTRY") == 0) {         
                    header.entry_point = instruction_addr;
                    found_entry = 1;
                }

                break;

            default:
                if (buffer_current - buffer_base > 63) 
                    ERROR("Token bigger than 64 bits")
                
                *buffer_current = *current;
                buffer_current++;
        }
        
        if (*current == '\0') {
            current = NULL;
            continue;
        }

        *buffer_current = '\0';
        current = (current==NULL) ? NULL : current+1;
    }


    // check entry
    if (!found_entry)
        ERROR("did not find the entry point, please set a 'ENTRY:' lable")

    // write program
    FILE* fp = fopen("out.wbin", "wb");
    if (fp == NULL)
        ERROR("can't open/create the output file")

    if (1 != fwrite(&header, sizeof(header), 1, fp))
        ERROR("can't write header")

    for (size_t i=0; i<program.len; i++) { 
        size_t size_out=0;
        char data_out[1024];
        operation_t op = program.operations[i];

        render_pgm(&op, &size_out, (char *)&data_out);

        if (1 != fwrite(data_out, size_out, 1, fp))
            ERROR("can't wriet instructions")
    }

    fclose(fp);

    INFO("ENDED")
    return 1;
}

