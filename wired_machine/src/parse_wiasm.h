#ifndef __PARSE_WIASM__
#define __PARSE_WIASM__

#include <stdint.h>
#include <string.h>

#include "utils/base_log.h"
#include "wired.def.h"

// parse args
void parse_scall_arg(char* buffer, unsigned int id, operation_t* op);
void parse_load_arg(char* buffer, unsigned int id, operation_t* op);


// parse 1 insttruction
size_t parse_instruction(char* buffer, program_t* pgm);


// helpers
void insert_instruction(program_t* pgm, operation_t op);

#endif