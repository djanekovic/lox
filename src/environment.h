#pragma once

#include <memory>
#include <unordered_map>

#include "value_type.h"
#include "lox.h"

namespace lox {
class Environment {
    Environment* enclosing_;
    std::unordered_map<std::string, ValueType> values_;

  public:
    // create a new environment enclosed with enclosing_env, I don't care
    // about that guys lifetime, he is my parent and he will outlive me
    Environment(Environment* enclosing):
        enclosing_{enclosing} {}

    Environment(): Environment(nullptr) {}

    void define(const std::string& name, ValueType value);
    ValueType get(const Token& name) const;
    void assign(const Token& name, ValueType value);
};
} //namespace lox
