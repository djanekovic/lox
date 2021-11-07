#pragma once

#include <memory>
#include "expr.h"

namespace lox {
struct StmtVisitor;
struct Stmt {
    virtual ~Stmt() = default;
    // accept method is const since we won't make changes to the Stmt object
    virtual void accept(StmtVisitor& visitor) const = 0;
};

struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;

struct StmtVisitor {
    virtual void visit_expression_stmt(const ExpressionStmt& node) = 0;
    virtual void visit_print_stmt(const PrintStmt& node) = 0;
    virtual void visit_var_stmt(const VarStmt& node) = 0;
};

struct ExpressionStmt: public Stmt {
    const std::unique_ptr<Expr> expression_;

    ExpressionStmt(std::unique_ptr<Expr> expression):
        expression_{std::move(expression)} {}

    void accept(StmtVisitor& visitor) const {
        return visitor.visit_expression_stmt(*this);
    }
};

struct PrintStmt : public Stmt {
    const std::unique_ptr<Expr> expression_;

    PrintStmt(std::unique_ptr<Expr> expression):
        expression_{std::move(expression)} {}

    void accept(StmtVisitor& visitor) const {
        return visitor.visit_print_stmt(*this);
    }
};

struct VarStmt: public Stmt {
    const Token name_;
    const std::unique_ptr<Expr> initializer_;

    VarStmt(Token name, std::unique_ptr<Expr> initializer):
        name_{std::move(name)}, initializer_{std::move(initializer)} {}

    void accept(StmtVisitor& visitor) const {
        return visitor.visit_var_stmt(*this);
    }
};


} //namespace lox
