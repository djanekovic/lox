#pragma once

#include <memory>
#include "../token.h"
#include "expr.h"

namespace lox {
struct UnaryExpr: public Expr {
    Token op_;
    std::unique_ptr<Expr> expr_;

    UnaryExpr(Token op, std::unique_ptr<Expr> expr):
        op_{std::move(op)}, expr_{std::move(expr)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_unary_node(*this);
    }
};
}
