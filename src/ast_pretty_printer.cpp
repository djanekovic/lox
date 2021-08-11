#include <fmt/core.h>
#include "ast_pretty_printer.h"

using namespace lox;

void ASTPrettyPrinter::visit_unary_node(const UnaryExpr& node)
{
    const char op = [](const auto op) {
        switch(op) {
            case UnaryExpr::UnaryOperator::MINUS:
                return '-';
            case UnaryExpr::UnaryOperator::BANG:
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
            case BinaryExpr::BinaryOperator::AND:
                return "and";
            case BinaryExpr::BinaryOperator::BANG_EQUAL:
                return "!=";
            case BinaryExpr::BinaryOperator::EQUAL_EQUAL:
                return "==";
            case BinaryExpr::BinaryOperator::GREATER:
                return ">";
            case BinaryExpr::BinaryOperator::GREATER_EQUAL:
                return ">=";
            case BinaryExpr::BinaryOperator::LESS:
                return "<";
            case BinaryExpr::BinaryOperator::LESS_EQUAL:
                return "<=";
            case BinaryExpr::BinaryOperator::MINUS:
                return "-";
            case BinaryExpr::BinaryOperator::OR:
                return "OR";
            case BinaryExpr::BinaryOperator::PLUS:
                return "+";
            case BinaryExpr::BinaryOperator::SLASH:
                return "/";
            case BinaryExpr::BinaryOperator::STAR:
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
