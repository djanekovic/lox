#pragma once

#include <memory>
#include "../token.h"
#include "expr.h"

namespace lox {
struct SetExpr: public Expr {
    Token name_;
    std::unique_ptr<Expr> object_;
    std::unique_ptr<Expr> value_;

    SetExpr(Token name, std::unique_ptr<Expr> object, std::unique_ptr<Expr> value):
        name_{std::move(name)}, object_{std::move(object)}, value_{std::move(value)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_set_node(*this);
    }
};
}
