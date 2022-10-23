#include <stdio.h>

#include "clox/debug.h"

static size_t simple_instruction(const char *name, size_t offset) {
  fprintf(stdout, "%s\n", name);
  return offset + 1;
}

static size_t constant_instruction(const char *name, Chunk *chunk, size_t offset) {
  uint8_t constant = chunk->code[offset + 1];
  fprintf(stdout, "%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  fprintf(stdout, "'\n");

  return offset + 2;
}

void disassemble_chunk(Chunk *chunk, const char *name) {
  fprintf(stdout, "== %s ==\n", name);
  for (size_t offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}

size_t disassemble_instruction(Chunk *chunk, size_t offset) {
  fprintf(stdout, "%04ld ", offset);
  uint8_t instr = chunk->code[offset];

  if ((offset > 0) && (chunk->lines[offset] == chunk->lines[offset - 1])) {
    fprintf(stdout, "   | ");
  } else {
    fprintf(stdout, "%4ld ", chunk->lines[offset]);
  }

  switch(instr) {
    case OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
      return simple_instruction("OP_RETURN", offset);
     default:
      fprintf(stderr, "Unknown opcode %d", instr);
      return offset + 1;
  }
}
