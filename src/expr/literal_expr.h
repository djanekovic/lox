#pragma once

#include "../value_type.h"
#include "../token.h"
#include "expr.h"

namespace lox {
struct LiteralExpr: public Expr {
    ValueType literal_;

    template<typename LiteralType>
    explicit LiteralExpr(LiteralType&& literal):
        literal_{std::forward<LiteralType>(literal)} {}

    void accept(ExprVisitor& visitor) const override {
        visitor.visit_literal_node(*this);
    }
};
}
