#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef double Value;

typedef struct {
  size_t capacity;
  size_t count;
  Value *values;
} ValueArray;

void init_value_array(ValueArray *value_array);
bool write_value_array(ValueArray *value_array, Value value);
void free_value_array(ValueArray *value_array);

void print_value(Value value);
