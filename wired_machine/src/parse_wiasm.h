#ifndef __PARSE_WIASM__
#define __PARSE_WIASM__

#include <stdint.h>
#include <string.h>

#include "utils/base_log.h"
#include "wired.def.h"

// parse args
void parse_scall_arg(char* buffer, unsigned int id, operation_t* op);

void parse_load_arg(char* buffer, unsigned int id, operation_t* op);
void parse_loadi_arg(char* buffer, unsigned int id, operation_t* op);
void parse_loada_arg(char* buffer, unsigned int id, operation_t* op);

void parse_save_arg(char* buffer, unsigned int id, operation_t* op);
void parse_savei_arg(char* buffer, unsigned int id, operation_t* op);
void parse_savea_arg(char* buffer, unsigned int id, operation_t* op);
void parse_saveai_arg(char* buffer, unsigned int id, operation_t* op);

void parse_add_arg(char* buffer, unsigned int id, operation_t* op);
void parse_addi_arg(char* buffer, unsigned int id, operation_t* op);
void parse_min_arg(char* buffer, unsigned int id, operation_t* op);
void parse_mini_arg(char* buffer, unsigned int id, operation_t* op);
void parse_mult_arg(char* buffer, unsigned int id, operation_t* op);
void parse_multi_arg(char* buffer, unsigned int id, operation_t* op);
void parse_div_arg(char* buffer, unsigned int id, operation_t* op);
void parse_divi_arg(char* buffer, unsigned int id, operation_t* op);


// parse 1 insttruction
size_t parse_instruction(char* buffer, program_t* pgm);


// helpers
void insert_instruction(program_t* pgm, operation_t op);

registry_t buffer_to_register(char* buffer);
unsigned int buffer_to_nb(char* buffer);
unsigned int buffer_to_hexnb(char* buffer);

#endif