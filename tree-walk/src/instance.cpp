#include <lox/instance.h>
#include <lox/class.h>

std::string lox::Instance::ToString() const {
    return fmt::format("{} instance", class_->GetName());
}


std::optional<lox::ValueType> lox::Instance::Get(const std::string& token_name) const {
    const auto it = fields_.find(token_name);
    if (it != std::cend(fields_)) {
        return it->second;
    }

    if (const auto method = class_->find_method(token_name)) {
        return std::make_shared<lox::Function>(*method);
    }

    return std::nullopt;
}
