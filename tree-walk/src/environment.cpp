#include <cassert>
#include "lox/environment.h"

using namespace lox;
void Environment::define(const std::string& name, ValueType value) {
    values_.insert_or_assign(name, value);
}

const Environment* Environment::ancestor(std::size_t distance) const {
    const Environment *env = this;
    for (auto i = 0U; i < distance; i++) {
        env = env->enclosing_.get();
    }

    return env;
}


Environment* Environment::ancestor(std::size_t distance) {
    Environment *env = this;
    for (int i = 0; i < distance; i++) {
        env = env->enclosing_.get();
    }

    return env;
}

ValueType Environment::get_at(std::size_t distance, const Token& name) const {
    const auto it = ancestor(distance)->values_.find(std::get<std::string>(name.lexeme_));
    assert(it != std::cend(values_));
    return it->second;
}

ValueType Environment::get(const Token& name) const {
    const auto it = values_.find(std::get<std::string>(name.lexeme_));
    if (it != std::cend(values_)) {
        return it->second;
    }

    // if we didn't find variable in current env, look up
    if (enclosing_) {
        return enclosing_->get(name);
    }

    throw Lox::RuntimeError(name, fmt::format("Undefined variable {}.", std::get<std::string>(name.lexeme_)));
}

void Environment::assign(const Token& name, ValueType value) {
    auto it = values_.find(std::get<std::string>(name.lexeme_));
    if (it != std::end(values_)) {
        it->second = value;
        return;
    }

    // if we can't assign in the current env, try up
    if (enclosing_) {
        enclosing_->assign(name, value);
        return;
    }

    throw Lox::RuntimeError(name, fmt::format("Undefined variable {}.", std::get<std::string>(name.lexeme_)));
}

void Environment::assign_at(std::size_t distance, const Token& name, ValueType value) {
    auto it = ancestor(distance)->values_.find(std::get<std::string>(name.lexeme_));
    assert(it != std::end(values_));
    it->second = value;
}
