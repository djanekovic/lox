#pragma once

namespace lox {

struct StmtVisitor;
struct Stmt {
    virtual ~Stmt() = default;
    // accept method is const since we won't make changes to the Stmt object
    virtual void accept(StmtVisitor& visitor) const = 0;
};

struct BlockStmt;
struct ExpressionStmt;
struct FunctionStmt;
struct IfExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct WhileStmt;

struct StmtVisitor {
    virtual void visit_block_stmt(const BlockStmt& node) = 0;
    virtual void visit_expression_stmt(const ExpressionStmt& node) = 0;
    virtual void visit_function_expression_stmt(const FunctionStmt& node) = 0;
    virtual void visit_if_expression_stmt(const IfExpressionStmt& node) = 0;
    virtual void visit_print_stmt(const PrintStmt& node) = 0;
    virtual void visit_var_stmt(const VarStmt& node) = 0;
    virtual void visit_while_stmt(const WhileStmt& node) = 0;
};
} //namespace lox
