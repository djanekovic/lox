#pragma once

#include <variant>
#include <string>

namespace lox {
// we are using monostate as nil!
using ValueType = std::variant<std::monostate, double, std::string, bool>;
} //namespace lox
