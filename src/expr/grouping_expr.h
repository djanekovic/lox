#pragma once

#include <memory>

#include "../token.h"
#include "expr.h"

namespace lox {
struct GroupingExpr: public Expr {
    std::unique_ptr<Expr> expr_;

    explicit GroupingExpr(std::unique_ptr<Expr> expr):
        expr_{std::move(expr)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_grouping_node(*this);
    }
};
}
