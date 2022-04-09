#pragma once

#include "class.h"

namespace lox {
class Instance {
  private:
    Class class_;
  public:
    Instance(Class& lox_class):
        class_{std::move(lox_class)} {}

    std::string ToString() const {
        return fmt::format("{} instance", class_.ToString());
    }
};
}
