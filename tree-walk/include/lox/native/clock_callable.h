#pragma once

#include "../value_type.h"

namespace lox {
struct ClockCallable final: public Callable {
    std::size_t arity() const { return 0; }
    ValueType operator()(Interpreter& interpreter, std::vector<ValueType>& arguments) const;
};
} // namespace lox
