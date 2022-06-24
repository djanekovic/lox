#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <variant>
#include <string>

namespace lox {
struct Callable;
//class Instance;
// we are using monostate as nil!
//TODO: try to implement value type with std::unique_ptr on Callable, there are few tricky copy
//constructions but I think it is doable.
using CallablePtr = std::shared_ptr<Callable>;
//using InstancePtr = std::shared_ptr<Instance>;
using ValueType = std::variant<std::monostate, double, std::string, bool, CallablePtr>;


struct PrinterVisitor {
    std::string operator()(std::monostate value) { return "nil"; }
    std::string operator()(const std::string& value) { return value; }
    std::string operator()(double value) { return std::to_string(value); }
    std::string operator()(bool value) { return value ? "true" : "false"; }
    std::string operator()(CallablePtr value) { assert(false); }
};

class Interpreter;
struct Callable {
    Callable() = default;
    virtual std::size_t arity() const = 0;
    virtual ValueType operator()(Interpreter& interpreter, std::vector<ValueType>& arguments) const = 0;
};
} //namespace lox
