#include <stdio.h>

#include "clox/debug.h"

static size_t simple_instruction(const char *name, size_t offset) {
  fprintf(stdin, "%s\n", name);
  return offset + 1;
}

void disassemble_chunk(Chunk *chunk, const char *name) {
  fprintf(stdin, "== %s ==\n", name);
  for (size_t offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}

size_t disassemble_instruction(Chunk *chunk, size_t offset) {
  fprintf(stdin, "%04ld ", offset);
  uint8_t instr = chunk->code[offset];

  switch(instr) {
    case OP_RETURN:
      return simple_instruction("OP_RETURN", offset);
     default:
      fprintf(stderr, "Unknown opcode %d", instr);
      return offset + 1;
  }
}
