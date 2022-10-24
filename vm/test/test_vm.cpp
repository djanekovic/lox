#include <gtest/gtest.h>

extern "C" {
#include "clox/chunk.h"
#include "clox/vm.h"
}

TEST(TestVM, BasicTestVM) {
  Chunk chunk;
  init_chunk(&chunk);
  init_vm();
  size_t constant = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant, 123);

  write_chunk(&chunk, OP_NEGATE, 123);

  write_chunk(&chunk, OP_RETURN, 123);

  interpret(&chunk);
  free_vm();
  free_chunk(&chunk);
}
