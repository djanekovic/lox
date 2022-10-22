#include <stdio.h>

#include "clox/memory.h"
#include "clox/chunk.h"

void init_chunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
}

bool write_chunk(Chunk *chunk, uint8_t byte) {
  // is there space for one more byte?
  if (chunk->capacity < chunk->count + 1) {
    size_t old_capacity = chunk->capacity;
    chunk->capacity = grow_capacity(old_capacity);
    if (!reallocate((void **) &chunk->code, old_capacity, chunk->capacity)) {
      fprintf(stderr, "Failed to grow chunk\n");
      return false;
    }
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;

  return true;
}

void free_chunk(Chunk *chunk) {
  reallocate((void **) &chunk->code, chunk->capacity, 0);
  init_chunk(chunk);
}
