#pragma once

#include <vector>
#include <map>

#include "expr/expr.h"
#include "stmt/stmt.h"

#include "interpreter.h"

namespace lox {
class Resolver: public ExprVisitor, public StmtVisitor {
private:
    enum FunctionType { NONE, FUNCTION };
    Interpreter &interpreter_;
    std::vector<std::map<std::string, bool>> scopes_;
    FunctionType current_function_{FunctionType::NONE};
public:
    explicit Resolver(Interpreter& interpreter):
        interpreter_{interpreter} {}

    void resolve(const std::vector<std::unique_ptr<Stmt>>& statements);
private:
    void visit_assign_node(const AssignExpr& node) override;
    void visit_literal_node(const LiteralExpr& node) override {}
    void visit_logical_node(const LogicalExpr& node) override;
    void visit_unary_node(const UnaryExpr& node) override;
    void visit_binary_node(const BinaryExpr& node) override;
    void visit_grouping_node(const GroupingExpr& node) override;
    void visit_variable_expr(const VariableExpr& node) override;
    void visit_call_expr(const CallExpr& node) override;
    void visit_get_expr(const GetExpr& node) override;
    void visit_block_stmt(const BlockStmt& stmt) override;

    void visit_class_stmt(const ClassStmt& stmt) override;
    void visit_function_expression_stmt(const FunctionStmt& stmt) override;
    void visit_expression_stmt(const ExpressionStmt& stmt) override;
    void visit_if_expression_stmt(const IfExpressionStmt& stmt) override;
    void visit_print_stmt(const PrintStmt& stmt) override;
    void visit_return_stmt(const ReturnStmt& stmt) override;
    void visit_var_stmt(const VarStmt& stmt) override;
    void visit_while_stmt(const WhileStmt& stmt) override;

    void resolve(const Stmt& statement);
    void resolve(const Expr& expression);
    void resolve_function(const FunctionStmt& stmt, FunctionType type);
    void resolve_local(const Expr& expr, const Token& name);
    void declare(const Token& name);
    void define(const Token& name);
    std::optional<bool> find_value_in_current_scope(const std::string& name) const;
    void begin_scope();
    void end_scope();
};
}
