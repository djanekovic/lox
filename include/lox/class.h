#pragma once

#include <string>
#include "interpreter.h"
#include "value_type.h"

namespace lox {
class Class final: public Callable {
  private:
    std::string name_;

  public:
    Class(std::string&& name):
        name_{std::move(name)} {}

    ValueType operator()(Interpreter& interpreter, std::vector<ValueType>& arguments) const override;

    std::string ToString() const {
        return name_;
    }

    std::size_t arity() const override { return 0; }
};
}
