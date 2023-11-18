#ifndef __WIRED_DEFS__
#define __WIRED_DEFS__

#include <stdint.h>

#define HEADER "WIRED"

typedef struct {
    uint8_t   identificator[5];
    uint64_t  ram_size;
    uint64_t  stack_size;
    uint64_t  entry_point;
} wired_vm_header_t;


// instructions data
typedef enum {
    SCALL_INST,
    LOAD_INST,
    LOADI_INST,

    NB_INST
} instruction_t;


// args
typedef struct 
{
    uint64_t arg0;
} inst_1_arg;

typedef struct 
{
    uint64_t arg0;
    uint64_t arg1;
} inst_2_arg;

typedef struct 
{
    uint64_t arg0;
    uint64_t arg1;
    uint64_t arg2;
} inst_3_arg;

typedef union {
    inst_1_arg arg_64;
    inst_2_arg arg_128;
    inst_3_arg arg_196;
} instruction_args_t;


typedef struct {
    uint16_t type;
    instruction_args_t args;
} operation_t;

#endif