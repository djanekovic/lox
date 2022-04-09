#pragma once

#include <memory>
#include <unordered_map>

#include "value_type.h"
#include "lox.h"

namespace lox {
class Environment: public std::enable_shared_from_this<Environment> {
    std::shared_ptr<Environment> enclosing_;
    std::unordered_map<std::string, ValueType> values_;

  public:
    // create a new environment enclosed with enclosing_env, I don't care
    // about that guys lifetime, he is my parent and he will outlive me
    Environment(std::shared_ptr<Environment> enclosing):
        enclosing_{std::move(enclosing)} {}

    Environment(): Environment(nullptr) {}

    void define(const std::string& name, ValueType value);
    ValueType get(const Token& name) const;
    ValueType get_at(std::size_t distance, const Token& name) const;
    void assign(const Token& name, ValueType value);
    void assign_at(std::size_t distance, const Token& name, ValueType value);

  private:
    const Environment* ancestor(std::size_t distance) const;
    Environment* ancestor(std::size_t distance);
};
} //namespace lox
