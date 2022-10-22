#include <stdio.h>
#include <stdlib.h>

#include "clox/memory.h"

#define MIN_CAPACITY 8
#define GROW_FACTOR 2

size_t grow_capacity(size_t old_capacity) {
  return old_capacity < MIN_CAPACITY ? MIN_CAPACITY : old_capacity * GROW_FACTOR;
}

bool reallocate(void **ptr, size_t old_size, size_t new_size) {
  // realloc api is a little bit weird. manpage says that the return
  // value if new_size=0 would be either NULL or a pointer suitable
  // to be passed to free. We would need to include that into our checks
  // after realloc and that would not be so pretty. Hence we are using just
  // the free for the case when new_size is 0.
  if (new_size == 0) {
    free(*ptr);
    return true;
  }

  void *new_ptr = realloc(*ptr, new_size);
  if (!new_ptr) {
    fprintf(stderr, "Realloc returned nullptr\n");
    return false;
  }

  *ptr = new_ptr;
  return true;
}

