#pragma once

#include <memory>
#include <unordered_map>

#include "value_type.h"
#include "lox.h"

namespace lox {
class Environment {
    std::unique_ptr<Environment> enclosing_;
    std::unordered_map<std::string, ValueType> values_;

  public:
    Environment() = default;
    Environment(std::unique_ptr<Environment> enclosing):
        enclosing_{std::move(enclosing)} {}

    void define(const std::string& name, ValueType value);
    ValueType get(const Token& name) const;
    void assign(const Token& name, ValueType value);
};
} //namespace lox
