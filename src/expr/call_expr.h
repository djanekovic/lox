#pragma once

#include <memory>
#include <vector>

#include "../token.h"
#include "expr.h"

namespace lox {
struct CallExpr: public Expr {
    std::unique_ptr<Expr> callee_;
    Token paren_;                       // store this token for location
    std::vector<std::unique_ptr<Expr>> arguments_;

    CallExpr(std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments):
        callee_{std::move(callee)}, paren_{std::move(paren)}, arguments_{std::move(arguments)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_call_expr(*this);
    }
};
}
