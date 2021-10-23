#include <cassert>
#include <type_traits>
#include <variant>

#include "interpreter.h"
#include "parser.h"

using namespace lox;
void Interpreter::visit_literal_node(const LiteralExpr& expr) {
    value_ = expr.literal_;
}

void Interpreter::visit_grouping_node(const GroupingExpr& expr) {
    // traverse down! -> value will be set when we reach literal node
    expr.expr_->accept(*this);
}

std::string Interpreter::stringify_value() {
    return std::visit(Token::PrinterVisitor(), value_);
}

struct TruthVisitor {
    template<typename T>
    bool operator()(T&& value) { return false; }
    bool operator()(bool value) { return value; }
};

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
    Interpreter::ValueType operator()(LhsType lhs, RhsType rhs) {
        throw Lox::RuntimeError(token_, "Operands must be either two strings or two numbers!");
    }

    Interpreter::ValueType operator()(const std::string& lhs, const std::string& rhs) { return lhs + rhs; }

    Interpreter::ValueType operator()(double lhs, double rhs) { return lhs + rhs; }
};

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
