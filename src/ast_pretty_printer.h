#pragma once

#include <string>
#include "expr/expr.h"

namespace lox {
class ASTPrettyPrinter: public ExprVisitor {
    std::string format;

public:
    ASTPrettyPrinter() = default;

    void visit_literal_node(const LiteralExpr& node) override;
    void visit_grouping_node(const GroupingExpr& node) override;
    void visit_unary_node(const UnaryExpr& node) override;
    void visit_binary_node(const BinaryExpr& node) override;

    std::string to_string() const { return format; }
};
} // namespace lox
