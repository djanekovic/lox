#include <cassert>
#include <type_traits>
#include <variant>

#include "lox/function.h"
#include "lox/return.h"
#include "lox/instance.h"
#include "lox/class.h"

#include "lox/expr/assign_expr.h"
#include "lox/expr/literal_expr.h"
#include "lox/expr/logical_expr.h"
#include "lox/expr/variable_expr.h"
#include "lox/expr/grouping_expr.h"
#include "lox/expr/call_expr.h"
#include "lox/expr/get_expr.h"
#include "lox/expr/unary_expr.h"
#include "lox/expr/binary_expr.h"

#include "lox/stmt/function_stmt.h"
#include "lox/stmt/class_stmt.h"
#include "lox/stmt/var_stmt.h"
#include "lox/stmt/block_stmt.h"
#include "lox/stmt/if_expression_stmt.h"
#include "lox/stmt/expression_stmt.h"
#include "lox/stmt/while_stmt.h"
#include "lox/stmt/print_stmt.h"
#include "lox/stmt/return_stmt.h"
#include "lox/interpreter.h"

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



struct PrinterVisitor {
    std::string operator()(std::monostate value) { return "nil"; }
    std::string operator()(const std::string& value) { return value; }
    std::string operator()(double value) { return std::to_string(value); }
    std::string operator()(bool value) { return value ? "true" : "false"; }
    std::string operator()(CallablePtr value) { assert(false); }
    std::string operator()(InstancePtr value) { return value->ToString(); }
    std::string operator()(ClassPtr value) { return value->ToString(); }
};



Interpreter::Interpreter():
    environment_{std::make_shared<Environment>()},
    globals_{environment_.get()} {
    // define native functions in global environment
    environment_->define("clock", std::make_shared<ClockCallable>());
}

Interpreter::~Interpreter() = default;

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

void Interpreter::resolve(const Expr& expr, std::size_t depth) {
    locals_.insert({&expr, depth});
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

void Interpreter::visit_class_stmt(const ClassStmt& stmt) {
    const auto class_name = std::get<std::string>(stmt.name_.lexeme_);
    environment_->define(class_name, std::monostate());
    auto lox_class = std::make_shared<Class>(class_name);
    environment_->assign(stmt.name_, lox_class);
}
void Interpreter::visit_expression_stmt(const ExpressionStmt& stmt) {
    evaluate(*stmt.expression_);
}

void Interpreter::visit_print_stmt(const PrintStmt& stmt) {
    evaluate(*stmt.expression_);
    fmt::print("{}\n", std::visit(PrinterVisitor(), value_));
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

    if (const auto it = locals_.find(&expr); it != std::cend(locals_)) {
        environment_->assign_at(it->second, expr.name_, value_);
    } else {
        globals_->assign(expr.name_, value_);
    }
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
    return std::visit(PrinterVisitor(), value_);
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
    auto callee = value_;

    std::vector<ValueType> arguments;
    arguments.reserve(node.arguments_.size());
    for (const auto& argument : node.arguments_) {
        evaluate(*argument);
        arguments.push_back(value_);
    }

    if (!std::holds_alternative<CallablePtr>(callee) and !std::holds_alternative<ClassPtr>(callee)) {
        throw Lox::RuntimeError(node.paren_, "Can only call functions and classes");
    }

    if (std::holds_alternative<ClassPtr>(callee)) {
        value_ = std::make_shared<Instance>(std::get<ClassPtr>(callee));
        return;
    }

    const auto& function = *std::get<CallablePtr>(callee);
    if (arguments.size() != function.arity()) {
        throw Lox::RuntimeError(node.paren_,
                fmt::format("Expected {} arguments but got {}.", function.arity(), arguments.size()));
    }

    value_ = function(*this, arguments);
}

#if 0
void Interpreter::visit_get_expr(const GetExpr& node) {
    evaluate(*node.object_);
    if (std::holds_alternative<InstancePtr>(value_)) {
        return std::get<InstancePtr>(value_)->get(std::get<std::string>(node.name_.lexeme_));
    }
}
#endif

void Interpreter::visit_variable_expr(const VariableExpr& node) {
    value_ = lookup_variable(node);
}

ValueType Interpreter::lookup_variable(const VariableExpr& node) {
    if (const auto it = locals_.find(&node); it != std::cend(locals_)) {
        return environment_->get_at(it->second, node.name_);
    } else {
        return globals_->get(node.name_);
    }
}
