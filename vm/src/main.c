#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "clox/chunk.h"
#include "clox/debug.h"
#include "clox/vm.h"

static void repl() {
  char *line = NULL;
  size_t n = 0;

  for (;;) {
    fprintf(stdout, "> ");
    if (getline(&line, &n, stdin) == -1) {
      fprintf(stdout, "\n");
      break;
    }

    interpret(line);
  }
}

static char *read_file(const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    fprintf(stderr, "fopen(%s) failed: %s", filename, strerror(errno));
    exit(1337);
  }
  fseek(f, 0L, SEEK_END);
  size_t file_size = ftell(f);
  rewind(f);

  char *buffer = malloc(file_size + 1);
  if (!buffer) {
    fprintf(stderr, "Not enough memory to read %s.", filename);
    exit(1337);
  }
  size_t bytes_read = fread(buffer, sizeof(char), file_size, f);
  if (bytes_read < file_size) {
    fprintf(stderr, "Could not read file %s", filename);
    exit(1337);
  }
  buffer[bytes_read] = '\0';

  fclose(f);
  return buffer;
}

static void run_file(const char *filename) {
  char *source = read_file(filename);
  InterpretResult result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR) exit(1337);
  if (result == INTERPRET_RUNTIME_ERROR) exit(1337);
}

int main(int argc, const char* argv[]) {
  init_vm();

  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    run_file(argv[1]);
  } else {
    fprintf(stderr, "Usage: %s [path]\n", argv[0]);
  }
  free_vm();
}
