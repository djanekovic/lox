#pragma once

#include "../token.h"
#include "expr.h"

namespace lox {
struct VariableExpr: public Expr {
    Token name_;

    VariableExpr(Token name): name_{std::move(name)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_variable_expr(*this);
    }
};
}
