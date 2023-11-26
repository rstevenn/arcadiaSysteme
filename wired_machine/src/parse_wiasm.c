#include "parse_wiasm.h"

// parse args

void parse_scall_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 0)
        ERROR("Too much args for a scall instr")

    unsigned int nb = 0;
    for (char* current=buffer; (*current) != '\0'; current++) {
        if (*current < '0' || *current > '9') 
            ERROR("Can't parse syscall number %s", buffer)

        nb *= 10;
        nb += *current-'0';
    }

    op->args.arg_64.arg0 = nb;
}

void parse_load_arg(char* buffer, unsigned int id, operation_t* op) {
    if (id > 1)
        ERROR("Too much args for a LOAD instr")

    ERROR("Not Implemented")
}




// parse 1 insttruction
size_t parse_instruction(char* buffer, program_t* pgm){
    if (strcmp("scall", buffer) == 0) {

        INFO("parsed scall")
        operation_t op;
        op.type = SCALL_INST;
        op.parse_args = (parse_args_t*)&parse_scall_arg;

        insert_instruction(pgm, op);
        return sizeof(raw_128_op);
    } 

    if (strcmp("load", buffer) == 0) {

        INFO("parsed load")
        operation_t op;
        op.type = LOAD_INST;
        op.parse_args = (parse_args_t*)&parse_load_arg;

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
