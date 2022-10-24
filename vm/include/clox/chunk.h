#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "clox/value.h"

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
  OP_NEGATE,
} OpCode;

typedef struct {
  size_t count;
  size_t capacity;
  uint8_t *code;
  ValueArray constants;
  size_t *lines;
} Chunk;

void init_chunk(Chunk *chunk);
bool write_chunk(Chunk *chunk, uint8_t byte, size_t line);
void free_chunk(Chunk *chunk);

size_t add_constant(Chunk* chunk, Value value);
