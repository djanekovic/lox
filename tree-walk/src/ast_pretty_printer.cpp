#include <fmt/core.h>

#include "lox/expr/unary_expr.h"
#include "lox/expr/binary_expr.h"
#include "lox/expr/literal_expr.h"
#include "lox/expr/grouping_expr.h"

#include "lox/ast_pretty_printer.h"

using namespace lox;

void ASTPrettyPrinter::visit_unary_node(const UnaryExpr& node)
{
    // traverse down
    node.expr_->accept(*this);
    format = fmt::format("({} {})", node.op_.stringify_token(), format);
}

void ASTPrettyPrinter::visit_binary_node(const BinaryExpr& node)
{
    // traverse the left side of the tree in DFS fashion
    node.lhs_->accept(*this);
    // save format string because right traversal will overwrite
    const std::string lhs_format = format;
    node.rhs_->accept(*this);

    format = fmt::format("({} {} {})", node.op_.stringify_token(), lhs_format, format);
}

void ASTPrettyPrinter::visit_literal_node(const LiteralExpr& node)
{
    format = std::visit(Token::PrinterVisitor(), node.literal_);
}

void ASTPrettyPrinter::visit_grouping_node(const GroupingExpr& node)
{
    node.expr_->accept(*this);
    format = fmt::format("(group {})", format);
}
