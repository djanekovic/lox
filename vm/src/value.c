#include <stdio.h>

#include "clox/memory.h"
#include "clox/value.h"

//TODO: this is a copy paste from chunk
void init_value_array(ValueArray *value_array) {
  value_array->count = 0;
  value_array->capacity = 0;
  value_array->values = NULL;
}

bool write_value_array(ValueArray *value_array, double value) {
  if (value_array->capacity < value_array->count + 1) {
    size_t old_capacity = value_array->capacity;
    value_array->capacity = grow_capacity(old_capacity);
    if (!reallocate((void **) &value_array->values, old_capacity * sizeof(Value), value_array->capacity * sizeof(Value))) {
      fprintf(stderr, "Failed to grow value array\n");
      return false;
    }
  }

  value_array->values[value_array->count] = value;
  value_array->count++;

  return true;
}

void free_value_array(ValueArray *value_array) {
  reallocate((void **) &value_array->values, value_array->capacity * sizeof(Value), 0);
  init_value_array(value_array);
}

void print_value(Value value) {
  fprintf(stdout, "%g", value);
}
