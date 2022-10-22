#pragma once

#include <stdbool.h>
#include <stddef.h>

size_t grow_capacity(size_t old_capacity);
bool reallocate(void **ptr, size_t old_size, size_t new_size);
