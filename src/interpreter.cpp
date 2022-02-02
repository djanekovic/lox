#include <cassert>
#include <type_traits>

#include "function.h"
#include "return.h"

#include "expr/assign_expr.h"
#include "expr/literal_expr.h"
#include "expr/logical_expr.h"
#include "expr/variable_expr.h"
#include "expr/grouping_expr.h"
#include "expr/call_expr.h"
#include "expr/unary_expr.h"
#include "expr/binary_expr.h"

#include "stmt/function_stmt.h"
#include "stmt/var_stmt.h"
#include "stmt/block_stmt.h"
#include "stmt/if_expression_stmt.h"
#include "stmt/expression_stmt.h"
#include "stmt/while_stmt.h"
#include "stmt/print_stmt.h"
#include "stmt/return_stmt.h"
#include "interpreter.h"

using namespace lox;

struct TruthVisitor {
    template<typename T>
    bool operator()(T&& value) { return true; }
    bool operator()(std::monostate value) { return false; }
    bool operator()(bool value) { return value; }
};

struct EqualsVisitor {
    // if the types are different just return false
    template<typename LhsType, typename RhsType>
    bool operator()(LhsType&& lhs, RhsType&& rhs) { return false; }

    bool operator()(std::monostate lhs, std::monostate rhs) { return true; }

    bool operator()(double lhs, double rhs) { return lhs == rhs; }

    bool operator()(bool lhs, bool rhs) { return lhs == rhs; }

    bool operator()(const std::string& lhs, const std::string& rhs) { return lhs == rhs; }
};

struct PlusVisitor {
    Token token_;
    PlusVisitor(const Token& token): token_{token} {}

    //TODO: we should use universal references here but we need SFINAE in that case.
    //because compiler will not call our string override
    template<typename LhsType, typename RhsType>
    ValueType operator()(LhsType lhs, RhsType rhs) {
        throw Lox::RuntimeError(token_, "Operands must be either two strings or two numbers!");
    }

    ValueType operator()(const std::string& lhs, const std::string& rhs) { return lhs + rhs; }

    ValueType operator()(double lhs, double rhs) { return lhs + rhs; }
};


void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>> statements) {
    try {
        for(const auto& statement: statements) {
            execute(*statement);
        }
    } catch (Lox::RuntimeError& ex) {
        Lox::runtime_error(ex);
    }
}


/** Handle statements **/
void Interpreter::execute(const Stmt& statement) {
    statement.accept(*this);
}

// execute this block in new environment
void Interpreter::execute_block(const std::vector<std::unique_ptr<Stmt>>& statements, std::shared_ptr<Environment>&& env) {
    // this is a cool RAII trick I got from here: https://github.com/eliasdaler/lox/blob/master/src/Interpreter.cpp#L376
    EnterEnvironmentGuard ee{*this, std::move(env)};
    for (const auto& stmt: statements) {
        execute(*stmt);
    }
}

void Interpreter::visit_function_expression_stmt(const FunctionStmt& stmt) {
    // here we are passing environment_ by value, causing ref count to increment
    // Function need to have it's closure even if interpreter is not using it anymore!
    auto function = std::make_shared<Function>(stmt, environment_);
    environment_->define(std::get<std::string>(stmt.name_.lexeme_), std::move(function));
}

void Interpreter::visit_block_stmt(const BlockStmt& stmt) {
    // create new environment where environment_ is parent environment(enclosure)
    execute_block(stmt.statements_, std::make_shared<Environment>(environment_));
}

void Interpreter::visit_expression_stmt(const ExpressionStmt& stmt) {
    evaluate(*stmt.expression_);
}

void Interpreter::visit_print_stmt(const PrintStmt& stmt) {
    evaluate(*stmt.expression_);
    fmt::print("{}\n", std::visit(Token::PrinterVisitor(), value_));
}

void Interpreter::visit_return_stmt(const ReturnStmt& stmt) {
    value_ = std::monostate();
    if (stmt.expression_) {
        evaluate(*stmt.expression_);
    }

    throw Return(value_);
}

void Interpreter::visit_if_expression_stmt(const IfExpressionStmt& stmt) {
    evaluate(*stmt.condition_);
    if (std::visit(TruthVisitor(), value_)) {
        execute(*stmt.then_stmt_);
    } else if (stmt.else_stmt_) {
        execute(*stmt.else_stmt_);
    }
}


void Interpreter::visit_var_stmt(const VarStmt& stmt) {
    if (stmt.initializer_) {
        evaluate(*stmt.initializer_);
    } else {
        // if there is no initializer, init to null
        value_ = std::monostate();
    }

    environment_->define(std::get<std::string>(stmt.name_.lexeme_), value_);
}

void Interpreter::visit_while_stmt(const WhileStmt& stmt) {
    for(;;) {
        evaluate(*stmt.condition_);
        if (!std::visit(TruthVisitor(), value_)) {
            return;
        }

        execute(*stmt.body_);
    }
}

void Interpreter::visit_assign_node(const AssignExpr& expr) {
    evaluate(*expr.value_);
    environment_->assign(expr.name_, value_);
}


/** Handle expressions **/
void Interpreter::evaluate(const Expr& expression) {
    expression.accept(*this);
}

void Interpreter::visit_literal_node(const LiteralExpr& expr) {
    value_ = expr.literal_;
}

void Interpreter::visit_logical_node(const LogicalExpr& expr) {
    evaluate(*expr.lhs_);

    if (expr.op_.type_ == TokenType::OR) {
        if (std::visit(TruthVisitor(), value_)) {
            // we have a or operation and first one is false, we can short circuit
            return;
        }
    } else {
        if (!std::visit(TruthVisitor(), value_)) {
            // we have an and operation and first one is false, we can short circuit
            return;
        }
    }

    evaluate(*expr.rhs_);
}

void Interpreter::visit_grouping_node(const GroupingExpr& expr) {
    // traverse down! -> value will be set when we reach literal node
    expr.expr_->accept(*this);
}

std::string Interpreter::stringify_value() const {
    return std::visit(Token::PrinterVisitor(), value_);
}

void Interpreter::visit_unary_node(const UnaryExpr& expr) {
    expr.expr_->accept(*this);

    switch(expr.op_.type_) {
        case TokenType::BANG:
            value_ = !std::visit(TruthVisitor(), value_);
            return;
        case TokenType::MINUS:
            check_number_operand(expr.op_, value_);
            value_ = - std::get<double>(value_);
            return;
        default:
            assert(false && "Something that is not UnaryOperator in visit_unary_node");
    }

    __builtin_unreachable();
}

void Interpreter::visit_binary_node(const BinaryExpr& expr) {
    expr.lhs_->accept(*this);
    const auto lhs = value_;
    expr.rhs_->accept(*this);

    switch(expr.op_.type_) {
        case TokenType::AND:
            value_ = std::get<bool>(lhs) and std::get<bool>(value_);
            return;
        case TokenType::BANG_EQUAL:
            value_ = !std::visit(EqualsVisitor(), lhs, value_);
            return;
        case TokenType::EQUAL_EQUAL:
            value_ = std::visit(EqualsVisitor(), lhs, value_);
            return;
        case TokenType::GREATER:
            check_number_operand(expr.op_, lhs, value_);
            value_ = std::get<double>(lhs) > std::get<double>(value_);
            return;
        case TokenType::GREATER_EQUAL:
            check_number_operand(expr.op_, lhs, value_);
            value_ = std::get<double>(lhs) >= std::get<double>(value_);
            return;
        case TokenType::LESS:
            check_number_operand(expr.op_, lhs, value_);
            value_ = std::get<double>(lhs) < std::get<double>(value_);
            return;
        case TokenType::LESS_EQUAL:
            check_number_operand(expr.op_, lhs, value_);
            value_ = std::get<double>(lhs) <= std::get<double>(value_);
            return;
        case TokenType::MINUS:
            check_number_operand(expr.op_, lhs, value_);
            value_ = std::get<double>(lhs) - std::get<double>(value_);
            return;
        case TokenType::OR:
            value_ = std::get<bool>(lhs) or std::get<bool>(value_);
            return;
        case TokenType::PLUS:
            value_ = std::visit(PlusVisitor(expr.op_), lhs, value_);
            return;
        case TokenType::SLASH:
            value_ = std::get<double>(lhs) / std::get<double>(value_);
            return;
        case TokenType::STAR:
            value_ = std::get<double>(lhs) * std::get<double>(value_);
            return;
        default:
            assert(false && "Something that is not BinaryOperator in visit_binary_node");
    }

    __builtin_unreachable();
}

void Interpreter::visit_call_expr(const CallExpr& node) {
    evaluate(*node.callee_);
    const auto callee = value_;

    std::vector<ValueType> arguments;
    arguments.reserve(node.arguments_.size());
    for (const auto& argument : node.arguments_) {
        evaluate(*argument);
        arguments.push_back(value_);
    }

    if (!std::holds_alternative<CallablePtr>(callee)) {
        throw Lox::RuntimeError(node.paren_, "Can only call functions and classes");
    }

    const auto& function = *std::get<CallablePtr>(callee);
    if (arguments.size() != function.arity()) {
        throw Lox::RuntimeError(node.paren_,
                fmt::format("Expected {} arguments but got {}.", function.arity(), arguments.size()));
    }

    value_ = function(*this, arguments);
}

void Interpreter::visit_variable_expr(const VariableExpr& node) {
    value_ = environment_->get(node.name_);
}
