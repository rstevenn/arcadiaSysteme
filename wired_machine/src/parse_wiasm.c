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

    registry_t reg = buffer_to_register(buffer);
    if (reg < 0)
        ERROR("Can't parse args for LOAD INST")


    if (id == 0)
        op->args.arg_128.arg0 = reg;
    if (id==1)
        op->args.arg_128.arg1 = reg;
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

    registry_t reg = buffer_to_register(buffer);
    if (reg < 0)
        ERROR("Can't parse args for SAVE INST")


    if (id == 0)
        op->args.arg_128.arg0 = reg;
    if (id==1)
        op->args.arg_128.arg1 = reg;
}


// parse 1 insttruction
size_t parse_instruction(char* buffer, program_t* pgm){
    if (strcmp("scall", buffer) == 0) {

        INFO("parsed scall")
        operation_t op;
        op.type = SCALL_INST;
        op.parse_args = (parse_args_t*)&parse_scall_arg;
        op.size = sizeof(raw_128_op);

        insert_instruction(pgm, op);
        return sizeof(raw_128_op);
    } 

    if (strcmp("load", buffer) == 0) {

        INFO("parsed load")
        operation_t op;
        op.type = LOAD_INST;
        op.parse_args = (parse_args_t*)&parse_load_arg;
        op.size = sizeof(raw_192_op);

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("loadi", buffer) == 0) {

        INFO("parsed loadi")
        operation_t op;
        op.type = LOADI_INST;
        op.parse_args = (parse_args_t*)&parse_loadi_arg;
        op.size = sizeof(raw_192_op);

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("loada", buffer) == 0) {

        INFO("parsed loada")
        operation_t op;
        op.type = LOADA_INST;
        op.parse_args = (parse_args_t*)&parse_loada_arg;
        op.size = sizeof(raw_192_op);

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
    }

    if (strcmp("save", buffer) == 0) {

        INFO("parsed save")
        operation_t op;
        op.type = SAVE_INST;
        op.parse_args = (parse_args_t*)&parse_save_arg;
        op.size = sizeof(raw_192_op);

        insert_instruction(pgm, op);
        return sizeof(raw_192_op);
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