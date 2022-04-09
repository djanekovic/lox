#pragma once

#include <memory>
#include "../token.h"
#include "stmt.h"

namespace lox {
struct Expr;
struct ReturnStmt : public Stmt {
    Token keyword_;
    std::unique_ptr<Expr> expression_;

    ReturnStmt(Token keyword, std::unique_ptr<Expr> expression):
        keyword_{std::move(keyword)}, expression_{std::move(expression)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_return_stmt(*this);
    }
};
}
