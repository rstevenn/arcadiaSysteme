#include "parse_wiasm.h"


// parse args
void parse_scall_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 0)
        ERROR("Too much args for a scall instr")

    op->args.arg_64.arg0 = buffer_to_nb(buffer);
}

void parse_load_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a LOAD instr")

    if (id == 0)
        op->args.arg_128.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_128.arg1 = buffer_to_register(buffer);
}

void parse_loadi_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a LOADI instr")

    if (id == 0) 
        op->args.arg_128.arg0 = buffer_to_register(buffer);
    if (id==1) 
         op->args.arg_128.arg1 = buffer_to_nb(buffer);
}

void parse_loada_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a LOADA instr")

    if (id == 0) 
        op->args.arg_128.arg0 = buffer_to_register(buffer);
    if (id==1) 
         op->args.arg_128.arg1 = buffer_to_hexnb(buffer);
}

void parse_save_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a SAVE instr")

    if (id == 0)
        op->args.arg_128.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_128.arg1 = buffer_to_register(buffer);
}

void parse_savei_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a SAVEI instr")

    if (id == 0)
        op->args.arg_128.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_128.arg1 = buffer_to_nb(buffer);
}

void parse_savea_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a SAVEA instr")

    if (id == 0)
        op->args.arg_128.arg0 = buffer_to_hexnb(buffer);
    if (id==1)
        op->args.arg_128.arg1 = buffer_to_register(buffer);
}

void parse_saveai_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a SAVEA instr")

    if (id == 0)
        op->args.arg_128.arg0 = buffer_to_hexnb(buffer);
    if (id==1)
        op->args.arg_128.arg1 = buffer_to_nb(buffer);
}

void parse_add_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a ADD instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_register(buffer);
}

void parse_addi_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a ADDI instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_nb(buffer);
}

void parse_min_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a MIN instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_register(buffer);
}

void parse_mini_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a MINI instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_nb(buffer);
}

void parse_mult_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a MULT instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_register(buffer);
}

void parse_multi_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a MULTI instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_nb(buffer);
}

void parse_div_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a DIV instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_register(buffer);
}

void parse_divi_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a DIVI instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_nb(buffer);
}

// parse 1 insttruction
size_t parse_instruction(char* buffer, program_t* pgm){
    if (strcmp("scall", buffer) == 0) {

        INFO("parsed scall")
        operation_t op;
        op.type = SCALL_INST;
        op.parse_args = (parse_args_t*)&parse_scall_arg;
        op.size = inst_128;

        insert_instruction(pgm, op);
        return sizeof(raw_128_op);
    } 

    if (strcmp("load", buffer) == 0) {

        INFO("parsed load")
        operation_t op;
        op.type = LOAD_INST;
        op.parse_args = (parse_args_t*)&parse_load_arg;
        op.size = inst_192;

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("loadi", buffer) == 0) {

        INFO("parsed loadi")
        operation_t op;
        op.type = LOADI_INST;
        op.parse_args = (parse_args_t*)&parse_loadi_arg;
        op.size = inst_192;

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("loada", buffer) == 0) {

        INFO("parsed loada")
        operation_t op;
        op.type = LOADA_INST;
        op.parse_args = (parse_args_t*)&parse_loada_arg;
        op.size = inst_192;

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("save", buffer) == 0) {

        INFO("parsed save")
        operation_t op;
        op.type = SAVE_INST;
        op.parse_args = (parse_args_t*)&parse_save_arg;
        op.size = inst_192;

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("savei", buffer) == 0) {

        INFO("parsed savei")
        operation_t op;
        op.type = SAVEI_INST;
        op.parse_args = (parse_args_t*)&parse_savei_arg;
        op.size = inst_192;

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("savea", buffer) == 0) {

        INFO("parsed savea")
        operation_t op;
        op.type = SAVEA_INST;
        op.parse_args = (parse_args_t*)&parse_savea_arg;
        op.size = inst_192;

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }
    
    if (strcmp("saveai", buffer) == 0) {

        INFO("parsed saveai")
        operation_t op;
        op.type = SAVEAI_INST;
        op.parse_args = (parse_args_t*)&parse_saveai_arg;
        op.size = inst_192;

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("add", buffer) == 0) {

        INFO("parsed add")
        operation_t op;
        op.type = ADD_INSTR;
        op.parse_args = (parse_args_t*)&parse_add_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    if (strcmp("addi", buffer) == 0) {

        INFO("parsed addi")
        operation_t op;
        op.type = ADDI_INSTR;
        op.parse_args = (parse_args_t*)&parse_addi_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    if (strcmp("min", buffer) == 0) {

        INFO("parsed min")
        operation_t op;
        op.type = MIN_INSTR;
        op.parse_args = (parse_args_t*)&parse_min_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    if (strcmp("mini", buffer) == 0) {

        INFO("parsed mini")
        operation_t op;
        op.type = MINI_INSTR;
        op.parse_args = (parse_args_t*)&parse_mini_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    if (strcmp("mult", buffer) == 0) {

        INFO("parsed mult")
        operation_t op;
        op.type = MULT_INSTR;
        op.parse_args = (parse_args_t*)&parse_mult_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    if (strcmp("multi", buffer) == 0) {

        INFO("parsed multi")
        operation_t op;
        op.type = ADDI_INSTR;
        op.parse_args = (parse_args_t*)&parse_multi_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    if (strcmp("div", buffer) == 0) {

        INFO("parsed div")
        operation_t op;
        op.type = MIN_INSTR;
        op.parse_args = (parse_args_t*)&parse_div_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    if (strcmp("divi", buffer) == 0) {

        INFO("parsed divi")
        operation_t op;
        op.type = MINI_INSTR;
        op.parse_args = (parse_args_t*)&parse_divi_arg;
        op.size = inst_256;

        insert_instruction(pgm, op);
        return sizeof(raw_256_op);
    }

    ERROR("Can't parse '%s'", buffer)   
}   


// helpers
void insert_instruction(program_t* pgm, operation_t op) {
    
    pgm->operations = (operation_t*)realloc(pgm->operations, sizeof(operation_t) * (pgm->len + 1));
    if (pgm->operations == NULL) 
        ERROR("Can't allocate memory")
    
    operation_t* ok = (operation_t*)memcpy(&(pgm->operations[pgm->len]), &op, sizeof(op));
    if (ok == NULL) 
        ERROR("Can't copy data")

    pgm->len++;
}

size_t new_instruction(program_t* pgm, instruction_t type, parse_args_t fnc, size_t size) {
    operation_t op;
    op.type = type;
    op.parse_args = (parse_args_t*)&parse_divi_arg;
    op.size = inst_256;

    insert_instruction(pgm, op);
    return size;
}

registry_t buffer_to_register(char* buffer){

    if (strcmp(buffer, "pc") == 0)
        return PC;
    if (strcmp(buffer, "eq") == 0)
        return EQ;
    if (strcmp(buffer, "sp") == 0)
        return SP;
    if (strcmp(buffer, "z") == 0)
        return Z;
    if (strcmp(buffer, "rt") == 0)
        return RT;

    if (strcmp(buffer, "ra") == 0)
        return RA;
    if (strcmp(buffer, "rb") == 0)
        return RB;
    if (strcmp(buffer, "rc") == 0)
        return RC;
    if (strcmp(buffer, "rd") == 0)
        return RD;
    if (strcmp(buffer, "re") == 0)
        return RE;
    if (strcmp(buffer, "rf") == 0)
        return RF;
    if (strcmp(buffer, "rg") == 0)
        return RG;
    if (strcmp(buffer, "rh") == 0)
        return RH;

    ERROR("Unkown register %s", buffer)
}

unsigned int buffer_to_nb(char* buffer) {
    unsigned int nb = 0;
    for (char* current=buffer; (*current) != '\0'; current++) {
        if (*current < '0' || *current > '9') 
            ERROR("Can't parse nb %s", buffer)

        nb *= 10;
        nb += *current-'0';
    }

    return nb;
}

unsigned int buffer_to_hexnb(char* buffer) {
    unsigned int nb = 0;
    
    if(*buffer != '0' || *(buffer+1) != 'x') {
        ERROR("can't parsse buffer as hex '%s'", buffer)
    } 

    buffer +=2;

    for (char* current=buffer; (*current) != '\0'; current++) {
        nb *= 16;
        if (*current >= '0' && *current <= '9') {
            nb += *current-'0'; 
        } else if (*current >= 'A' && *current <= 'F') {
            nb += *current-'A'+10; 
        } else if (*current >= 'a' && *current <= 'f') {
            nb += *current-'a'+10; 
        }else {
            ERROR("can't parsse buffer as hex '%s'", buffer)
        }
    }

    return nb;
}