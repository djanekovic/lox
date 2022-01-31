#pragma once

#include <memory>

#include "stmt.h"

namespace lox {
struct Expr;
struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> expression_;

    PrintStmt(std::unique_ptr<Expr> expression):
        expression_{std::move(expression)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_print_stmt(*this);
    }
};
}
