#pragma once

#include <vector>
#include <memory>

#include "../token.h"
#include "stmt.h"

namespace lox {
struct FunctionStmt: public Stmt {
    Token name_;
    std::vector<Token> params_;
    std::vector<std::unique_ptr<Stmt>> body_;

    FunctionStmt(Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body):
        name_{std::move(name)}, params_{std::move(params)}, body_{std::move(body)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_function_expression_stmt(*this);
    }
};
}
