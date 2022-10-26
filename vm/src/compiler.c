#include <stddef.h>
#include <stdio.h>

#include "clox/scanner.h"
#include "clox/compiler.h"

void compile(const char *source) {
  init_scanner(source);

  size_t line = -1;
  for (;;) {
    Token token = scan_token();
    if (token.line != line) {
      printf("%4ld ", token.line);
      line = token.line;
    } else {
      printf("    | ");
    }

    fprintf(stdout, "%2d '%.*s'\n", token.type, token.length, token.start);

    if (token.type == TOKEN_EOF) break;
  }
}
