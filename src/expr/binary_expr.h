#pragma once

#include <memory>
#include "../token.h"
#include "expr.h"

namespace lox {
struct BinaryExpr: public Expr {
    Token op_;
    std::unique_ptr<Expr> lhs_;
    std::unique_ptr<Expr> rhs_;

    BinaryExpr(Token op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs):
        op_{std::move(op)}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_binary_node(*this);
    }
};
}
