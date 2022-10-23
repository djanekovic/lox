#include <stdio.h>

#include "clox/memory.h"
#include "clox/chunk.h"

void init_chunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;

  init_value_array(&chunk->constants);
}

bool write_chunk(Chunk *chunk, uint8_t byte, size_t line) {
  // is there space for one more byte?
  if (chunk->capacity < chunk->count + 1) {
    size_t old_capacity = chunk->capacity;
    chunk->capacity = grow_capacity(old_capacity);
    if (!reallocate((void **) &chunk->code, old_capacity * sizeof(uint8_t), chunk->capacity * sizeof(uint8_t))) {
      fprintf(stderr, "Failed to grow chunk\n");
      return false;
    }


    if (!reallocate((void **) &chunk->lines, old_capacity * sizeof(size_t), chunk->capacity * sizeof(size_t))) {
      fprintf(stderr, "Failed to grow lines\n");
      return false;
    }
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;

  return true;
}

void free_chunk(Chunk *chunk) {
  reallocate((void **) &chunk->code, chunk->capacity * sizeof(uint8_t), 0);
  reallocate((void **) &chunk->lines, chunk->capacity * sizeof(size_t), 0);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

size_t add_constant(Chunk* chunk, Value value) {
  //TODO: error handle write fail
  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;
}
