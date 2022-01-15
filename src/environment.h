#pragma once

#include <unordered_map>

#include "value_type.h"
#include "lox.h"

namespace lox {
class Environment {
    std::unordered_map<std::string, ValueType> values_;

  public:
    void define(const std::string& name, ValueType value) {
        values_.insert_or_assign(name, value);
    }

    ValueType get(const Token& name) const {
        const auto it = values_.find(std::get<std::string>(name.lexeme_));
        if (it != std::cend(values_)) {
            return it->second;
        }

        throw Lox::RuntimeError(name, fmt::format("Undefined variable {}.", std::get<std::string>(name.lexeme_)));
    }

    void assign(const Token& name, ValueType value) {
        auto it = values_.find(std::get<std::string>(name.lexeme_));
        if (it != std::end(values_)) {
            it->second = value;
            return;
        }

        throw Lox::RuntimeError(name, fmt::format("Undefined variable {}.", std::get<std::string>(name.lexeme_)));
    }

};
} //namespace lox
