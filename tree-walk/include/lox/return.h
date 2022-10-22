#pragma once

#include <stdexcept>
#include "value_type.h"

namespace lox {
struct Return: public std::exception {
    ValueType value_;
    Return(ValueType value): value_{std::move(value)} {}
};
}
