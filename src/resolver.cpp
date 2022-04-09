#include <algorithm>
#include <optional>

#include "lox/expr/assign_expr.h"
#include "lox/expr/literal_expr.h"
#include "lox/expr/logical_expr.h"
#include "lox/expr/variable_expr.h"
#include "lox/expr/grouping_expr.h"
#include "lox/expr/call_expr.h"
#include "lox/expr/get_expr.h"
#include "lox/expr/unary_expr.h"
#include "lox/expr/binary_expr.h"

#include "lox/stmt/class_stmt.h"
#include "lox/stmt/function_stmt.h"
#include "lox/stmt/var_stmt.h"
#include "lox/stmt/block_stmt.h"
#include "lox/stmt/if_expression_stmt.h"
#include "lox/stmt/expression_stmt.h"
#include "lox/stmt/while_stmt.h"
#include "lox/stmt/print_stmt.h"
#include "lox/stmt/return_stmt.h"

#include "lox/resolver.h"

using namespace lox;

void Resolver::visit_assign_node(const AssignExpr& node) {
    resolve(*node.value_);
    resolve_local(node, node.name_);
}

void Resolver::visit_logical_node(const LogicalExpr& node) {
    resolve(*node.lhs_);
    resolve(*node.rhs_);
}

void Resolver::visit_unary_node(const UnaryExpr& node) {
    resolve(*node.expr_);
}
void Resolver::visit_binary_node(const BinaryExpr& node) {
    resolve(*node.lhs_);
    resolve(*node.rhs_);
}

void Resolver::visit_grouping_node(const GroupingExpr& node) {
    resolve(*node.expr_);
}

void Resolver::visit_variable_expr(const VariableExpr& node) {
    if (!scopes_.empty()) {
        if (const auto v = find_value_in_current_scope(std::get<std::string>(node.name_.lexeme_))) {
            if (!v.value()) {
                Lox::error(node.name_, "Can't read local variable in its own initializer!");
            }
        }
    }

    resolve_local(node, node.name_);
}

void Resolver::visit_call_expr(const CallExpr& node) {
    resolve(*node.callee_);

    std::for_each(std::cbegin(node.arguments_), std::cend(node.arguments_),
            [&](const auto& argument) { resolve(*argument); });
}

void Resolver::visit_get_expr(const GetExpr& node) {
    resolve(*node.object_);
}

void Resolver::visit_block_stmt(const BlockStmt& stmt) {
    begin_scope();
    resolve(stmt.statements_);
    end_scope();
}

void Resolver::visit_function_expression_stmt(const FunctionStmt& stmt) {
    declare(stmt.name_);
    define(stmt.name_);

    resolve_function(stmt, FunctionType::FUNCTION);
}

void Resolver::visit_expression_stmt(const ExpressionStmt& stmt) {
    resolve(*stmt.expression_);
}

void Resolver::visit_if_expression_stmt(const IfExpressionStmt& stmt) {
    resolve(*stmt.condition_);
    resolve(*stmt.then_stmt_);
    if (stmt.else_stmt_) {
        resolve(*stmt.else_stmt_);
    }
}

void Resolver::visit_class_stmt(const ClassStmt& stmt) {
    declare(stmt.name_);
    define(stmt.name_);
}

void Resolver::visit_print_stmt(const PrintStmt& stmt) {
    resolve(*stmt.expression_);
}

void Resolver::visit_return_stmt(const ReturnStmt& stmt) {
    if (current_function_ == FunctionType::NONE) {
        Lox::error(stmt.keyword_, "Return statement not inside the function!");
    }

    if (stmt.expression_) {
        resolve(*stmt.expression_);
    }
}

void Resolver::visit_var_stmt(const VarStmt& stmt) {
    declare(stmt.name_);
    if (stmt.initializer_) {
        resolve(*stmt.initializer_);
    }

    define(stmt.name_);
}

void Resolver::visit_while_stmt(const WhileStmt& stmt) {
    resolve(*stmt.condition_);
    resolve(*stmt.body_);
}

/**
 * PRIVATE FUNCTIONS
 */


void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& statements) {
    std::for_each(std::cbegin(statements), std::cend(statements), [&](const auto& s) { resolve(*s); });
}

void Resolver::resolve(const Stmt& statement) {
    statement.accept(*this);
}

void Resolver::resolve(const Expr& expression) {
    expression.accept(*this);
}

void Resolver::resolve_function(const FunctionStmt& stmt, FunctionType type) {
    //TODO: std::exchange
    FunctionType enclosing_function = current_function_;
    current_function_ = type;

    begin_scope();
    std::for_each(std::cbegin(stmt.params_), std::cend(stmt.params_),
                  [&](const auto& param) {
                      declare(param);
                      define(param);
                  });
    resolve(stmt.body_);
    end_scope();

    current_function_ = enclosing_function;
}

void Resolver::resolve_local(const Expr& expr, const Token& name) {
    // start the search in the innermost scope
    const auto it = std::find_if(std::crbegin(scopes_), std::crend(scopes_),
            [&](const auto& s) { return s.find(std::get<std::string>(name.lexeme_)) != std::cend(s); });
    // we found the variable, resolve it in interpreter
    if (it != std::crend(scopes_)) {
        // I am a bit lazy to figure out how to compare reverse and forward iterators.. :)
        interpreter_.resolve(expr, std::distance(std::crbegin(scopes_), it));
    }
}

void Resolver::declare(const Token& name) {
    if (scopes_.empty()) {
        return;
    }

    auto& scope = scopes_.back();

    if (scope.find(std::get<std::string>(name.lexeme_)) != std::cend(scope)) {
        Lox::error(name, "Already a variable with this name in this scope");
    }

    // not ready yet
    scope.emplace(std::make_pair(std::get<std::string>(name.lexeme_), false));
}

void Resolver::define(const Token& name) {
    if (scopes_.empty()) {
        return;
    }

    scopes_.back().insert_or_assign(std::get<std::string>(name.lexeme_), true);
}

std::optional<bool> Resolver::find_value_in_current_scope(const std::string& name) const {
    const auto it = scopes_.back().find(name);
    return it != std::cend(scopes_.back()) ? std::make_optional(it->second) : std::nullopt;
}

void Resolver::begin_scope() {
    scopes_.emplace_back();
}

void Resolver::end_scope() {
    scopes_.erase(std::cend(scopes_) - 1);
}
