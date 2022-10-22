#pragma once

#include <memory>
#include <vector>

#include "../token.h"
#include "expr.h"

namespace lox {
struct GetExpr: public Expr {
    std::unique_ptr<Expr> object_;
    Token name_;

    GetExpr(std::unique_ptr<Expr> object, Token name):
        object_{std::move(object)}, name_{std::move(name)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_get_expr(*this);
    }
};
}
