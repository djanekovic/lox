#include <gtest/gtest.h>

extern "C" {
#include "clox/chunk.h"
}

TEST(TestChunk, BasicTestChunk) {
  Chunk chunk;
  init_chunk(&chunk);
  ASSERT_EQ(chunk.count, 0);
  ASSERT_EQ(chunk.capacity, 0);
  write_chunk(&chunk, OP_RETURN, 1);
  free_chunk(&chunk);
}


TEST(TestChunk, GrowTestChunk) {
  Chunk chunk;
  init_chunk(&chunk);
  for (int i = 0; i < 9; i++) {
    write_chunk(&chunk, OP_RETURN, i);
  }

  ASSERT_EQ(chunk.count, 9);
  ASSERT_EQ(chunk.lines[8], 8);
  ASSERT_EQ(chunk.capacity, 16);
  free_chunk(&chunk);
}

TEST(TestChunk, BasicConstantTest) {
  Chunk chunk;
  init_chunk(&chunk);
  size_t constant = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant, 123);

  write_chunk(&chunk, OP_RETURN, 123);

  EXPECT_EQ(chunk.code[0], OP_CONSTANT);
  EXPECT_EQ(chunk.code[1], 0);
  EXPECT_DOUBLE_EQ(chunk.constants.values[0], 1.2);

  EXPECT_EQ(chunk.code[2], OP_RETURN);
}
