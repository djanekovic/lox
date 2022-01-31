#pragma once

#include <memory>

#include "stmt.h"

namespace lox {
struct Expr;
struct IfExpressionStmt: public Stmt {
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> then_stmt_;
    std::unique_ptr<Stmt> else_stmt_;

    IfExpressionStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_stmt):
        IfExpressionStmt(std::move(condition), std::move(then_stmt), nullptr) {}

    IfExpressionStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_stmt, std::unique_ptr<Stmt> else_stmt):
        condition_{std::move(condition)}, then_stmt_{std::move(then_stmt)}, else_stmt_{std::move(else_stmt)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_if_expression_stmt(*this);
    }
};
}
