#pragma once

#include "stmt/function_stmt.h"
#include "interpreter.h"
#include "value_type.h"
#include "return.h"

namespace lox {
class Function final: public Callable {
    const FunctionStmt& declaration_;   // save a refernce because I know Interpreter won't call delete on the statement
    std::shared_ptr<Environment> closure_;
public:
    Function(const FunctionStmt& function, std::shared_ptr<Environment> closure):
        declaration_{function}, closure_{std::move(closure)} {}

    ValueType operator()(Interpreter& interpreter, std::vector<ValueType>& arguments) const override {
        auto env = std::make_shared<Environment>(closure_);
        for (auto i = 0U; i < declaration_.params_.size(); i++) {
            env->define(std::get<std::string>(declaration_.params_.at(i).lexeme_), arguments.at(i));
        }

        try {
            interpreter.execute_block(declaration_.body_, std::move(env));
        } catch (const Return& return_value) {
            return return_value.value_;
        }

        return std::monostate();
    }

    std::size_t arity() const override { return declaration_.params_.size(); }
};
}
