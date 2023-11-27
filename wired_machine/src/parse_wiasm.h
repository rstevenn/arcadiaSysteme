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

void parse_and_arg(char* buffer, unsigned int id, operation_t* op);
void parse_andi_arg(char* buffer, unsigned int id, operation_t* op);
void parse_or_arg(char* buffer, unsigned int id, operation_t* op);
void parse_ori_arg(char* buffer, unsigned int id, operation_t* op);
void parse_xor_arg(char* buffer, unsigned int id, operation_t* op);
void parse_xori_arg(char* buffer, unsigned int id, operation_t* op);
void parse_not_arg(char* buffer, unsigned int id, operation_t* op);
void parse_noti_arg(char* buffer, unsigned int id, operation_t* op);

void parse_eq_arg(char* buffer, unsigned int id, operation_t* op);
void parse_eqi_arg(char* buffer, unsigned int id, operation_t* op);
void parse_neq_arg(char* buffer, unsigned int id, operation_t* op);
void parse_neqi_arg(char* buffer, unsigned int id, operation_t* op);

void parse_gt_arg(char* buffer, unsigned int id, operation_t* op);
void parse_gti_arg(char* buffer, unsigned int id, operation_t* op);
void parse_gte_arg(char* buffer, unsigned int id, operation_t* op);
void parse_gtei_arg(char* buffer, unsigned int id, operation_t* op);

void parse_lt_arg(char* buffer, unsigned int id, operation_t* op);
void parse_lti_arg(char* buffer, unsigned int id, operation_t* op);
void parse_lte_arg(char* buffer, unsigned int id, operation_t* op);
void parse_ltei_arg(char* buffer, unsigned int id, operation_t* op);

void parse_jre_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jrei_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jeq_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jeqi_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jne_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jnei_arg(char* buffer, unsigned int id, operation_t* op);

void parse_jmp_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jmpi_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jmp_th_arg(char* buffer, unsigned int id, operation_t* op);
void parse_jmp_thi_arg(char* buffer, unsigned int id, operation_t* op);


// parse 1 insttruction
size_t parse_instruction(char* buffer, program_t* pgm);


// helpers
void insert_instruction(program_t* pgm, operation_t op);
size_t new_instruction(program_t* pgm, instruction_t type, parse_args_t* fnc, raw_instr_type raw_instr_type);

registry_t buffer_to_register(char* buffer);
unsigned int buffer_to_nb(char* buffer);
unsigned int buffer_to_hexnb(char* buffer);
int is_hexnb(char* buffer);

#endif