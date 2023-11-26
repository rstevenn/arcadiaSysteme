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

void parse_and_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a AND instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_register(buffer);
}

void parse_andi_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a ANDI instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_nb(buffer);
}

void parse_or_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a OR instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_register(buffer);
}

void parse_ori_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a ORI instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_nb(buffer);
}

void parse_xor_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a XOR instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_register(buffer);
}

void parse_xori_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 2)
        ERROR("Too much args for a XORI instr")

    if (id == 0)
        op->args.arg_192.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_192.arg1 = buffer_to_register(buffer);
    if (id==2)
        op->args.arg_192.arg2 = buffer_to_nb(buffer);
}

void parse_not_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 1)
        ERROR("Too much args for a XOR instr")

    if (id == 0)
        op->args.arg_128.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_128.arg1 = buffer_to_register(buffer);
}

void parse_noti_arg(char* buffer, unsigned int id, operation_t* op){
    if (id > 1)
        ERROR("Too much args for a XORI instr")

    if (id == 0)
        op->args.arg_128.arg0 = buffer_to_register(buffer);
    if (id==1)
        op->args.arg_128.arg1 = buffer_to_nb(buffer);
}

// parse 1 insttruction
size_t parse_instruction(char* buffer, program_t* pgm){
    if (strcmp("scall", buffer) == 0) {

        INFO("parsed scall")
        return new_instruction(pgm, SCALL_INST,
                               (parse_args_t*)&parse_scall_arg, 
                               inst_128);
    } 

    if (strcmp("load", buffer) == 0) {

        INFO("parsed load")
        return new_instruction(pgm, LOAD_INST,
                                (parse_args_t*)&parse_load_arg,
                                inst_192);
    }

    if (strcmp("loadi", buffer) == 0) {

        INFO("parsed loadi")
        return new_instruction(pgm, LOADI_INST,
                                (parse_args_t*)&parse_loadi_arg,
                                inst_192);
    }

    if (strcmp("loada", buffer) == 0) {

        INFO("parsed loada")
        return new_instruction(pgm, LOADA_INST,
                                (parse_args_t*)&parse_loada_arg,
                                inst_192);
    }

    if (strcmp("save", buffer) == 0) {

        INFO("parsed save")
        return new_instruction(pgm, SAVE_INST,
                                (parse_args_t*)&parse_save_arg,
                                inst_192);
    }

    if (strcmp("savei", buffer) == 0) {

        INFO("parsed savei")
        return new_instruction(pgm, SAVEI_INST,
                                (parse_args_t*)&parse_savei_arg,
                                inst_192);
    }

    if (strcmp("savea", buffer) == 0) {

        INFO("parsed savea")
        return new_instruction(pgm, SAVEA_INST,
                                (parse_args_t*)&parse_savea_arg,
                                inst_192);
    }
    
    if (strcmp("saveai", buffer) == 0) {

        INFO("parsed saveai")
        return new_instruction(pgm, SAVEAI_INST,
                                (parse_args_t*)&parse_saveai_arg,
                                inst_192);
    }

    if (strcmp("add", buffer) == 0) {

        INFO("parsed add")
        return new_instruction(pgm, ADD_INST,
                                (parse_args_t*)&parse_add_arg,
                                inst_256);
    }

    if (strcmp("addi", buffer) == 0) {

        INFO("parsed addi")
        return new_instruction(pgm, ADDI_INST,
                                (parse_args_t*)&parse_addi_arg,
                                inst_256);
    }

    if (strcmp("min", buffer) == 0) {

        INFO("parsed min")
        return new_instruction(pgm, MIN_INST,
                                (parse_args_t*)&parse_min_arg,
                                inst_256);
    }

    if (strcmp("mini", buffer) == 0) {

        INFO("parsed mini")
        return new_instruction(pgm, MINI_INST,
                                (parse_args_t*)&parse_mini_arg,
                                inst_256);
    }

    if (strcmp("mult", buffer) == 0) {

        INFO("parsed mult")
        return new_instruction(pgm, MULT_INST,
                                (parse_args_t*)&parse_mult_arg,
                                inst_256);
    }

    if (strcmp("multi", buffer) == 0) {

        INFO("parsed multi")
        return new_instruction(pgm, MULTI_INST,
                                (parse_args_t*)&parse_multi_arg,
                                inst_256);
    }

    if (strcmp("div", buffer) == 0) {

        INFO("parsed div")
        return new_instruction(pgm, DIV_INST,
                                (parse_args_t*)&parse_div_arg,
                                inst_256);
    }

    if (strcmp("divi", buffer) == 0) {

        INFO("parsed divi")
        return new_instruction(pgm, DIVI_INST,
                                (parse_args_t*)&parse_divi_arg,
                                inst_256);
    }

    if (strcmp("and", buffer) == 0) {

        INFO("parsed and")
        return new_instruction(pgm, AND_INST,
                                (parse_args_t*)&parse_and_arg,
                                inst_256);
    }

    if (strcmp("andi", buffer) == 0) {

        INFO("parsed andi")
        return new_instruction(pgm, ANDI_INST,
                                (parse_args_t*)&parse_andi_arg,
                                inst_256);
    }

    if (strcmp("or", buffer) == 0) {

        INFO("parsed or")
        return new_instruction(pgm, OR_INST,
                                (parse_args_t*)&parse_or_arg,
                                inst_256);
    }

    if (strcmp("ori", buffer) == 0) {

        INFO("parsed ori")
        return new_instruction(pgm, ORI_INST,
                                (parse_args_t*)&parse_ori_arg,
                                inst_256);
    }

    if (strcmp("xor", buffer) == 0) {

        INFO("parsed xor")
        return new_instruction(pgm, XOR_INST,
                                (parse_args_t*)&parse_xor_arg,
                                inst_256);
    }

    if (strcmp("xori", buffer) == 0) {

        INFO("parsed xori")
        return new_instruction(pgm, XORI_INST,
                                (parse_args_t*)&parse_xori_arg,
                                inst_256);
    }

    if (strcmp("not", buffer) == 0) {

        INFO("parsed not")
        return new_instruction(pgm, NOT_INST,
                                (parse_args_t*)&parse_not_arg,
                                inst_192);
    }

    if (strcmp("noti", buffer) == 0) {

        INFO("parsed noti")
        return new_instruction(pgm, NOTI_INST,
                                (parse_args_t*)&parse_noti_arg,
                                inst_192);
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

size_t new_instruction(program_t* pgm, instruction_t type, parse_args_t* fnc, raw_instr_type raw_instr_type) {
    operation_t op;
    op.type = type;
    op.parse_args = fnc;
    op.size = raw_instr_type;

    insert_instruction(pgm, op);
    
    if (raw_instr_type==inst_128)
        return sizeof(raw_128_op);

    if (raw_instr_type==inst_192)
        return sizeof(raw_192_op);
    
    if (raw_instr_type==inst_256)
        return sizeof(raw_256_op);
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