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
    SAVEI_INST = 0x05,
    SAVEA_INST = 0x06,
    SAVEAI_INST = 0x07,

    ADD_INST = 0x08,
    ADDI_INST = 0x09,
    MIN_INST = 0x0a,
    MINI_INST = 0x0b,
    MULT_INST = 0x0c,
    MULTI_INST = 0x0d,
    DIV_INST = 0x0e,
    DIVI_INST = 0x0f,

    AND_INST = 0x10,
    ANDI_INST = 0x11,
    OR_INST = 0x12,
    ORI_INST = 0x13,
    XOR_INST = 0x14,
    XORI_INST = 0x15,
    NOT_INST = 0x16,
    NOTI_INST = 0x17,

    EQ_INST = 0x18,
    EQI_INST = 0x19,
    NEQ_INST = 0x1a,
    NEQI_INST = 0x1b,

    GT_INST = 0x1c,
    GTI_INST = 0x1d,
    GTE_INST = 0x1e,
    GTEI_INST = 0x1f,

    LT_INST = 0x20,
    LTI_INST = 0x21,
    LTE_INST = 0x22,
    LTEI_INST = 0x23,

    JRE_INST = 0x24,
    JREI_INST = 0x25,
    JEQ_INST = 0x26,
    JEQI_INST = 0x27,
    JNE_INST = 0x28,
    JNEI_INST = 0x29,

    JMP_INST = 0x2a,
    JMPI_INST = 0x2b,
    JMP_TH_INST = 0x2c,
    JMP_THI_INST = 0x2d,

} instruction_t;

typedef enum {
    // system registry
    PC = 0,
    EQ, SP, Z, RT,
    
    // user registry
    RA, RB, RC, RD, 
    RE, RF, RG, RH,

    // hiden system regiters
    SPL,

    REGISTERS_NB
} registry_t;

typedef enum {
    inst_128,
    inst_192,
    inst_256,
    inst_jmp
    
    

} raw_instr_type;


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
    uint32_t op_code;
    uint32_t op_size;
    inst_1_arg args;
} raw_128_op;

typedef struct  {
    uint32_t op_code;
    uint32_t op_size;
    inst_2_arg args;
} raw_192_op;

typedef struct  {
    uint32_t op_code;
    uint32_t op_size;
    inst_3_arg args;
} raw_256_op;


///
// assembleur struct
///

typedef struct _operation_t operation_t;
typedef void(parse_args_t)(char* buffer, int id, operation_t* op);

struct _operation_t{
    uint16_t type;
    raw_instr_type size;
    instruction_args_t args;
    parse_args_t* parse_args;
};


typedef struct  _program_t {
    
    size_t len;
    operation_t* operations;
} program_t;

///
// vm struct
///

typedef struct {
    uint32_t op_code;
    uint32_t op_size;
} op_meta_t;

typedef struct {
    op_meta_t meta;
    uint64_t args[3];
} vm_op_t;


#endif
