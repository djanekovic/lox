#pragma once

#include <string>
#include "stmt/stmt.h"
#include "expr/expr.h"

namespace lox {
class ASTPrettyPrinter: public lox::ExprVisitor, public lox::StmtVisitor {
public:
    ASTPrettyPrinter() = default;
    std::string to_string() const { return format; }

private:
    void visit_assign_node(const lox::AssignExpr& node) override;
    void visit_binary_node(const lox::BinaryExpr& node) override;
    void visit_call_expr(const lox::CallExpr& node) override;
    void visit_get_expr(const lox::GetExpr& node) override;
    void visit_grouping_node(const lox::GroupingExpr& node) override;
    void visit_literal_node(const lox::LiteralExpr& node) override;
    void visit_logical_node(const lox::LogicalExpr& node) override;
    void visit_unary_node(const lox::UnaryExpr& node) override;
    void visit_variable_expr(const lox::VariableExpr& node) override;

    void visit_block_stmt(const lox::BlockStmt& node) override;
    void visit_class_stmt(const lox::ClassStmt& node) override;
    void visit_expression_stmt(const lox::ExpressionStmt& node) override;
    void visit_function_expression_stmt(const lox::FunctionStmt& node) override;
    void visit_if_expression_stmt(const lox::IfExpressionStmt& node) override;
    void visit_print_stmt(const lox::PrintStmt& node) override;
    void visit_return_stmt(const lox::ReturnStmt& node) override;
    void visit_var_stmt(const lox::VarStmt& node) override;
    void visit_while_stmt(const lox::WhileStmt& node) override;

    std::string format;
};
} // namespace lox
