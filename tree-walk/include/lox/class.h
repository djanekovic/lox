#pragma once

#include <optional>
#include <string>
#include <lox/function.h>
#include "interpreter.h"

namespace lox {
class Class final {
  public:
    Class(const std::string& name, const std::map<std::string, Function>& methods):
        name_{name}, methods_{methods} {}

    std::string_view GetName() const { return name_; }
    std::optional<Function> find_method(const std::string& method_name) const {
        const auto it = methods_.find(method_name);
        return it != std::cend(methods_) ? std::make_optional(it->second) : std::nullopt;
    }

    std::string ToString() const;

  private:
    std::string name_;
    std::map<std::string, Function> methods_;
};
}
