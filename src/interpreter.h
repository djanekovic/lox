#pragma once

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

#include "lox.h"
#include "expr/expr.h"
#include "stmt/stmt.h"

#include "native/clock_callable.h"

#include "environment.h"

namespace lox {
class Interpreter: public ExprVisitor, public StmtVisitor {
    class EnterEnvironmentGuard {
        Interpreter& interpreter_;
        std::unique_ptr<Environment> previous_;

    public:
        EnterEnvironmentGuard(Interpreter& interpreter, std::unique_ptr<Environment>&& env):
            interpreter_{interpreter} // take reference of the current interpreter
            // old interpreter.environment_ goes to previous and env goes in his place
            //previous_{std::exchange(interpreter_.environment_, std::move(env))} {}
        {
            previous_ = std::move(interpreter_.environment_);
            interpreter_.environment_ = std::move(env);
        }

        ~EnterEnvironmentGuard() {
            // reset previous environment
            interpreter_.environment_ = std::move(previous_);
        }
    };


    ValueType value_;
    std::unique_ptr<Environment> globals_;       // global Environment
    std::unique_ptr<Environment> environment_;   // current Environment

    void execute(const Stmt& statement);
    void visit_assign_node(const AssignExpr& node) override;
    void visit_literal_node(const LiteralExpr& node) override;
    void visit_logical_node(const LogicalExpr& node) override;
    void visit_unary_node(const UnaryExpr& node) override;
    void visit_binary_node(const BinaryExpr& node) override;
    void visit_grouping_node(const GroupingExpr& node) override;
    void visit_variable_expr(const VariableExpr& node) override;
    void visit_call_expr(const CallExpr& node) override;

    void evaluate(const Expr& expression);
    void visit_block_stmt(const BlockStmt& stmt) override;
    void visit_function_expression_stmt(const FunctionStmt& stmt) override;
    void visit_expression_stmt(const ExpressionStmt& stmt) override;
    void visit_if_expression_stmt(const IfExpressionStmt& stmt) override;
    void visit_print_stmt(const PrintStmt& stmt) override;
    void visit_var_stmt(const VarStmt& stmt) override;
    void visit_while_stmt(const WhileStmt& stmt) override;

    std::string stringify_value() const;

    template<typename OperatorType>
    static void check_number_operand(OperatorType&& op, ValueType v) {
        if (std::holds_alternative<double>(v)) {
            return;
        }
        throw Lox::RuntimeError(op, "Operand must be a number!");
    }


    template<typename OperatorType>
    static void check_number_operand(OperatorType&& op, ValueType lhs, ValueType rhs) {
        if (std::holds_alternative<double>(lhs) and std::holds_alternative<double>(rhs)) {
            return;
        }
        throw Lox::RuntimeError(op, "Operands must be a number!");
    }

  public:
    Interpreter():
        globals_{std::make_unique<Environment>()},
        environment_{std::make_unique<Environment>(*globals_)} {
        globals_->define("clock", std::make_shared<ClockCallable>());
    }

    void interpret(std::vector<std::unique_ptr<Stmt>> statements);

    void execute_block(const std::vector<std::unique_ptr<Stmt>>& statements, std::unique_ptr<Environment>&& env);
    Environment *get_globals_ptr() const { return globals_.get(); }
};
} // namespace lox
