#include <gtest/gtest.h>

extern "C" {
#include "clox/chunk.h"
}

TEST(TestChunk, BasicTestChunk) {
  Chunk chunk;
  init_chunk(&chunk);
  ASSERT_EQ(chunk.count, 0);
  ASSERT_EQ(chunk.capacity, 0);
  write_chunk(&chunk, OP_RETURN);
  free_chunk(&chunk);
}


TEST(TestChunk, GrowTestChunk) {
  Chunk chunk;
  init_chunk(&chunk);
  for (int i = 0; i < 9; i++) {
    write_chunk(&chunk, OP_RETURN);
  }

  ASSERT_EQ(chunk.count, 9);
  ASSERT_EQ(chunk.capacity, 16);
  free_chunk(&chunk);
}
