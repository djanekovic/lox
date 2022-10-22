#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef enum {
  OP_RETURN,
} OpCode;

typedef struct {
  size_t count;
  size_t capacity;
  uint8_t *code;
} Chunk;

void init_chunk(Chunk *chunk);
bool write_chunk(Chunk *chunk, uint8_t byte);
void free_chunk(Chunk *chunk);
