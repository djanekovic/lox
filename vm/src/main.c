#include "clox/chunk.h"
#include "clox/debug.h"

#include "clox/vm.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  init_chunk(&chunk);
  init_vm();
  size_t constant = add_constant(&chunk, 1.2);
  write_chunk(&chunk, OP_CONSTANT, 123);
  write_chunk(&chunk, constant, 123);

  write_chunk(&chunk, OP_RETURN, 123);

  interpret(&chunk);
  free_vm();
  free_chunk(&chunk);
}
