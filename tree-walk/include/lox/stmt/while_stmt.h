#pragma once

#include <memory>
#include "stmt.h"

namespace lox {
struct Expr;
struct WhileStmt: public Stmt {
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> body_;

    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body):
        condition_{std::move(condition)}, body_{std::move(body)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_while_stmt(*this);
    }
};
}
