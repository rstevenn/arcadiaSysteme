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
    SCALL_INST = 0,
    LOAD_INST,

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
    inst_3_arg arg_192;
    char* flag;
} instruction_args_t;

// raw opreations
typedef struct  {
    uint64_t op_code;
    inst_1_arg args;
} raw_128_op;

typedef struct  {
    uint64_t op_code;
    inst_2_arg args;
} raw_192_op;

typedef struct  {
    uint64_t op_code;
    inst_3_arg args;
} raw_256_op;


// assembleur struct
typedef struct _operation_t operation_t;
typedef void(parse_args_t)(char* buffer, int id, operation_t* op);

struct _operation_t{
    uint16_t type;
    instruction_args_t args;
    parse_args_t* parse_args;
};


typedef struct  _program_t {
    
    size_t len;
    operation_t* operations;
} program_t;

#endif