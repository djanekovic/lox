#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>
#include <fmt/core.h>

#include "lox.h"
#include "stmt.h"

namespace lox {
class Interpreter: public Visitor, public StmtVisitor {
  public:
    using ValueType = LiteralExpr::LiteralType;

    void interpret(std::vector<std::unique_ptr<Stmt>> statements) {
        try {
            for(auto& statement: statements) {
                execute(std::move(statement));
            }
        } catch (Lox::RuntimeError& ex) {
            Lox::runtime_error(ex);
        }
    }

  private:

    ValueType value_;

    // Statement lives till the end of this function
    void execute(std::unique_ptr<Stmt> statement) {
        statement->accept(*this);
    }

    void visit_literal_node(const LiteralExpr& node) override;
    void visit_unary_node(const UnaryExpr& node) override;
    void visit_binary_node(const BinaryExpr& node) override;
    void visit_grouping_node(const GroupingExpr& node) override;

    void visit_expression_stmt(const ExpressionStmt& stmt) override {
        evaluate(*stmt.expression_);
    }

    void visit_print_stmt(const PrintStmt& stmt) override {
        evaluate(*stmt.expression_);
        fmt::print("{}\n", std::visit(Token::PrinterVisitor(), value_));
    }

    void evaluate(const Expr& expression) {
        expression.accept(*this);
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
