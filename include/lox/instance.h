#pragma once
#include <map>
#include <memory>
#include <string>

#include <lox/value_type.h>

namespace lox {
class Class;

class Instance {
  public:
    Instance(std::shared_ptr<Class> lox_class):
        class_{lox_class} {}

    std::string ToString() const;
    std::optional<ValueType> Get(const std::string& token_name) const;

    void Set(const std::string& token_name, ValueType value) {
        fields_.emplace(token_name, value);
    }

  private:
    std::shared_ptr<Class> class_;
    std::map<std::string, ValueType> fields_;
};
}
