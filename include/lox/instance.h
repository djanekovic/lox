#pragma once
#include <memory>
#include <string>

namespace lox {
class Class;

class Instance {
  public:
    Instance(std::shared_ptr<Class> lox_class):
        class_{lox_class} {}

    std::string ToString() const;

  private:
    std::shared_ptr<Class> class_;
};
}
