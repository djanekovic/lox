#pragma once

#include <memory>
#include "stmt.h"

namespace lox {
struct Expr;
struct ExpressionStmt: public Stmt {
    std::unique_ptr<Expr> expression_;

    ExpressionStmt(std::unique_ptr<Expr> expression):
        expression_{std::move(expression)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_expression_stmt(*this);
    }
};
}
