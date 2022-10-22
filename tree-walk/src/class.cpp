#include <lox/class.h>

std::string lox::Class::ToString() const {
    return fmt::format("{} class", name_);
}
