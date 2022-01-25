#pragma once

#include <memory>
#include <vector>
#include "expr.h"

namespace lox {
struct StmtVisitor;
struct Stmt {
    virtual ~Stmt() = default;
    // accept method is const since we won't make changes to the Stmt object
    virtual void accept(StmtVisitor& visitor) const = 0;
};

struct BlockStmt;
struct ExpressionStmt;
struct IfExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct WhileStmt;

struct StmtVisitor {
    virtual void visit_block_stmt(const BlockStmt& node) = 0;
    virtual void visit_expression_stmt(const ExpressionStmt& node) = 0;
    virtual void visit_if_expression_stmt(const IfExpressionStmt& node) = 0;
    virtual void visit_print_stmt(const PrintStmt& node) = 0;
    virtual void visit_var_stmt(const VarStmt& node) = 0;
    virtual void visit_while_stmt(const WhileStmt& node) = 0;
};

struct BlockStmt: public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements_;

    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>>&& statements):
        statements_{std::move(statements)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_block_stmt(*this);
    }
};

struct ExpressionStmt: public Stmt {
    std::unique_ptr<Expr> expression_;

    ExpressionStmt(std::unique_ptr<Expr> expression):
        expression_{std::move(expression)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_expression_stmt(*this);
    }
};

struct IfExpressionStmt: public Stmt {
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> then_stmt_;
    std::unique_ptr<Stmt> else_stmt_;

    IfExpressionStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_stmt):
        IfExpressionStmt(std::move(condition), std::move(then_stmt), nullptr) {}

    IfExpressionStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_stmt, std::unique_ptr<Stmt> else_stmt):
        condition_{std::move(condition)}, then_stmt_{std::move(then_stmt)}, else_stmt_{std::move(else_stmt)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_if_expression_stmt(*this);
    }
};

struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> expression_;

    PrintStmt(std::unique_ptr<Expr> expression):
        expression_{std::move(expression)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_print_stmt(*this);
    }
};

struct VarStmt: public Stmt {
    Token name_;
    std::unique_ptr<Expr> initializer_;

    VarStmt(Token name, std::unique_ptr<Expr> initializer):
        name_{std::move(name)}, initializer_{std::move(initializer)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_var_stmt(*this);
    }
};

struct WhileStmt: public Stmt {
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> body_;

    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body):
        condition_{std::move(condition)}, body_{std::move(body)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_while_stmt(*this);
    }
};
} //namespace lox
