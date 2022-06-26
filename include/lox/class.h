#pragma once

#include <string>
#include "interpreter.h"

namespace lox {
class Class final {
  public:
    Class(const std::string& name):
        name_{name} {}

    std::string_view GetName() const { return name_; }

    std::string ToString() const;

  private:
    std::string name_;
};
}
