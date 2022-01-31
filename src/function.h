#pragma once

#include "stmt.h"
#include "interpreter.h"
#include "value_type.h"

namespace lox {
class Function final: public Callable {
    const FunctionStmt& declaration_;   // save a refernce because I know Interpreter won't call delete on the statement
public:
    Function(const FunctionStmt& function):
        declaration_{function} {}

    ValueType operator()(Interpreter& interpreter, std::vector<ValueType>& arguments) const override {
        auto env = std::make_unique<Environment>(interpreter.get_globals_ptr());
        for (int i = 0; i < declaration_.params_.size(); i++) {
            env->define(std::get<std::string>(declaration_.params_.at(i).lexeme_), arguments.at(i));
        }
        interpreter.execute_block(declaration_.body_, std::move(env));
        return std::monostate();
    }

    std::size_t arity() const override { return declaration_.params_.size(); }
};
}
