#include "expr.h"
#include <fmt/core.h>

void ASTPrettyPrinter::visit_unary_node(const UnaryExpr& node)
{
    const char op = [](const auto op) {
        switch(op) {
            case UnaryOperator::MINUS:
                return '-';
            case UnaryOperator::BANG:
                return '!';
        }
        __builtin_unreachable();
    }(node.op_);

    // traverse down
    node.expr_->accept(*this);
    format = fmt::format("({} {})", op, format);
}

void ASTPrettyPrinter::visit_binary_node(const BinaryExpr& node)
{
    const char *op = [](const auto op) {
        switch(op) {
            case BinaryOperator::AND:
                return "and";
            case BinaryOperator::BANG_EQUAL:
                return "!=";
            case BinaryOperator::EQUAL_EQUAL:
                return "==";
            case BinaryOperator::GREATER:
                return ">";
            case BinaryOperator::GREATER_EQUAL:
                return ">=";
            case BinaryOperator::LESS:
                return "<";
            case BinaryOperator::LESS_EQUAL:
                return "<=";
            case BinaryOperator::MINUS:
                return "-";
            case BinaryOperator::OR:
                return "OR";
            case BinaryOperator::PLUS:
                return "+";
            case BinaryOperator::SLASH:
                return "/";
            case BinaryOperator::STAR:
                return "*";

        }
        __builtin_unreachable();
    }(node.op_);

    // traverse the left side of the tree in DFS fashion
    node.lhs_->accept(*this);
    // save format string because right traversal will overwrite
    const std::string lhs_format = format;
    node.rhs_->accept(*this);

    format = fmt::format("({} {} {})", op, lhs_format, format);
}

void ASTPrettyPrinter::visit_literal_node(const LiteralExpr& node)
{
    format = node.literal_;
}

void ASTPrettyPrinter::visit_grouping_node(const GroupingExpr& node)
{
    node.expr_->accept(*this);
    format = fmt::format("(group {})", format);
}
