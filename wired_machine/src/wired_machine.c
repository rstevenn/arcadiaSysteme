#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/base_log.h"
#include "wired.def.h"

char *readAllFile(char *path, size_t *file_size_out) {
  // open file
  FILE *fp = fopen(path, "rb");
  CHECK_ALLOCATE(fp, "Can't read the file %s", path);

  // get file size
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  *file_size_out = size;

  // read data
  char *buffer = (char *)malloc(sizeof(char) * (size + 1));
  CHECK_ALLOCATE(buffer, "Unable to allocate a buffer of %lu chars", size)

  size_t got;
  CHECK_READ_WRITE(size, got = fread(buffer, sizeof(char), size, fp),
                   "unable to read the file %s (expected %lu != got %lu)", path,
                   size, got);
  buffer[got] = '\0';

  // close file
  fclose(fp);

  return buffer;
}

int main(int argc, char *argv[]) {
  // check args
  if (argc <= 1)
    ERROR("No file pass as arg")

  // read file
  size_t file_size;
  char *rawText = readAllFile(argv[1], &file_size);
  char *current = rawText;
  INFO("LOAD: '%s' %lu bits", argv[1], file_size)

  // extract and sanitize header metadata
  if (file_size < sizeof(wired_vm_header_t))
    ERROR("the file is to short to contain a header")

  wired_vm_header_t header = *(wired_vm_header_t *)current;
  current += sizeof(header);
  size_t pgm_size = file_size - (current - rawText);

  if (strcmp("WIRE", (const char *)header.identificator) != 0)
    ERROR("Invalid indentificator")

  if (header.ram_size < pgm_size + header.stack_size)
    ERROR("Not enough ram allocated")

  if (header.entry_point > pgm_size)
    ERROR("Entry point out of executable zone");

  // setup ram
  INFO("init ram of %lu bits and load program", header.ram_size);
  char *vm_ram = (char *)malloc(header.ram_size * sizeof(char));
  if (vm_ram == NULL)
    ERROR("Can't allocate ram memory")

  if (memcpy(vm_ram, current, pgm_size * sizeof(char)) == NULL)
    ERROR("Can't copy program on ram")

  // setup registers
  INFO("setup registers")
  uint64_t registers[REGISTERS_NB] = {0};
  registers[SPL] = header.ram_size - 1;
  registers[SP] = header.ram_size - 1 - header.stack_size;
  registers[PC] = header.entry_point;

  // run
  INFO("Launch programm")

  while (1) {
    // fetch
    INFO("fetch at pc: %lu", registers[PC])
    char *pc = vm_ram + registers[PC];
    op_meta_t op_meta = *(op_meta_t *)pc;
    vm_op_t op = {0};
    char pc_set = 0;

    switch (op_meta.op_size) {
    case (inst_128): {
      INFO("op_size: 128 bits")
      op.meta = op_meta;
      op.args[0] = *(uint64_t *)(pc + sizeof(op_meta));
      break;
    }

    case (inst_192): {
      INFO("op_size: 192 bits")
      op.meta = op_meta;
      op.args[0] = *(uint64_t *)(pc + sizeof(op_meta));
      op.args[1] = *(uint64_t *)(pc + sizeof(op_meta) + sizeof(uint64_t));
      break;
    }

    case (inst_256): {
      INFO("op_size: 256 bits")
      op.meta = op_meta;
      op.args[0] = *(uint64_t *)(pc + sizeof(op_meta));
      op.args[1] = *(uint64_t *)(pc + sizeof(op_meta) + sizeof(uint64_t));
      op.args[2] = *(uint64_t *)(pc + sizeof(op_meta) + 2 * sizeof(uint64_t));
      break;
    }

    default:
      ERROR("Invalide op size '%ul'", op.meta.op_size)
    }

    // decode && execute
    switch (op.meta.op_code) {
    case (SCALL_INST): {
      INFO("SCALL 0x%04lx", op.args[0])

      switch (op.args[0]) {
      case (0): {
        INFO("no op")
        break;
      }

      default:
        WARNING("Invalid scall number 0x%04lx", op.args[0])
        break;
      }

      break;
    }

    // LOAD
    case (LOAD_INST): {
        INFO("LOAD")
        registers[op.args[0]] = registers[op.args[1]];
        break;
    }

    case (LOADI_INST): {
        INFO("LOADI")
        registers[op.args[0]] = op.args[1];
        break;
    }

    case (LOADA_INST): {
        INFO("LOADA")
        registers[op.args[0]] = *(vm_ram+op.args[1]);
        break;
    }

    // SAVE
    case (SAVE_INST): {
        INFO("SAVE")
        *(vm_ram+registers[op.args[0]]) = registers[op.args[1]];
        break;
    }

    case (SAVEI_INST): {
        INFO("SAVEI")
        *(vm_ram+registers[op.args[0]]) = op.args[1];
        break;
    }
    
    case (SAVEA_INST): {
        INFO("SAVEA")
        *(vm_ram+op.args[0]) = registers[op.args[1]];
        break;
    }

    case (SAVEAI_INST): {
        INFO("SAVEAI")
        *(vm_ram+op.args[0]) = op.args[1];
        break;
    }





    default:
      ERROR("Unknow op code 0x%04x", op.meta.op_code)
    }

    // clock
    if (!pc_set) {
      switch (op_meta.op_size) {
      case (inst_128): {
        registers[PC] += 2 * sizeof(uint64_t);
        break;
      }

      case (inst_192): {
        registers[PC] += 3 * sizeof(uint64_t);
        break;
      }

      case (inst_256): {
        registers[PC] += 4 * sizeof(uint64_t);
        break;
      }

      default:
        ERROR("Invalide op size '%ul'", op.meta.op_size)
      }
    }
  }

  // clear
  free(vm_ram);
  free(rawText);
  INFO("END")
  return 0;
}
