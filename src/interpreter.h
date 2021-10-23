#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <cassert>
#include <fmt/core.h>

#include "lox.h"
#include "expr.h"

namespace lox {
class Interpreter: public Visitor {
  public:
    using ValueType = LiteralExpr::LiteralType;

    void interpret(std::unique_ptr<Expr> expression) {
        try {
            evaluate(std::move(expression));
            fmt::print("{}\n", stringify_value());
        } catch (Lox::RuntimeError& ex) {
            Lox::runtime_error(ex);
        }
    }

  private:

    ValueType value_;

    void visit_literal_node(const LiteralExpr& node) override;
    void visit_unary_node(const UnaryExpr& node) override;
    void visit_binary_node(const BinaryExpr& node) override;
    void visit_grouping_node(const GroupingExpr& node) override;

    void evaluate(std::unique_ptr<Expr> expression) {
        expression->accept(*this);
    }

    std::string stringify_value();

    template<typename OperatorType>
    static void check_number_operand(OperatorType&& op, Interpreter::ValueType v) {
        if (std::holds_alternative<double>(v)) { return; }
        throw Lox::RuntimeError(op, "Operand must be a number!");
    }


    template<typename OperatorType>
    static void check_number_operand(OperatorType&& op, Interpreter::ValueType lhs, Interpreter::ValueType rhs) {
        if (std::holds_alternative<double>(lhs) and std::holds_alternative<double>(rhs)) { return; }
        throw Lox::RuntimeError(op, "Operands must be a number!");
    }
};
} // namespace lox
