#include <lox/instance.h>
#include <lox/class.h>

std::string lox::Instance::ToString() const {
    return fmt::format("{} instance", class_->GetName());
}
