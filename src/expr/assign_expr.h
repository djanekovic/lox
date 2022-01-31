#pragma once

#include <memory>
#include "../token.h"
#include "expr.h"

namespace lox {
struct AssignExpr: public Expr {
    Token name_;
    std::unique_ptr<Expr> value_;

    AssignExpr(Token name_, std::unique_ptr<Expr> value):
        name_{std::move(name_)}, value_{std::move(value)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_assign_node(*this);
    }
};
}
