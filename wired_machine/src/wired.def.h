#ifndef __WIRED_DEFS__
#define __WIRED_DEFS__

#include <stdint.h>

#define HEADER "WIRED"

/// 
// HEADER
///
typedef struct {
    uint8_t   identificator[5];
    uint64_t  ram_size;
    uint64_t  stack_size;
    uint64_t  entry_point;
} wired_vm_header_t;

///
// enumes
///

typedef enum {
    SCALL_INST = 0x00,
    LOAD_INST  = 0x01,
    LOADI_INST = 0x02,
    LOADA_INST = 0x03,

    SAVE_INST = 0x04,


} instruction_t;

typedef enum {
    // system registry
    PC = 0,
    EQ, SP, Z, RT,
    
    // user registry
    RA, RB, RC, RD, 
    RE, RF, RG, RH,

    REISTRY_NB
} registry_t;


///
// args
///
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
    char flag[128];
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


///
// assembleur struct
///

typedef struct _operation_t operation_t;
typedef void(parse_args_t)(char* buffer, int id, operation_t* op);

struct _operation_t{
    uint16_t type;
    uint64_t size;
    instruction_args_t args;
    parse_args_t* parse_args;
};


typedef struct  _program_t {
    
    size_t len;
    operation_t* operations;
} program_t;

#endif