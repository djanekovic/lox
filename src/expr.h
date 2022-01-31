#pragma once
#include <memory>
#include <vector>
#include <string>

#include "token.h"

namespace lox {
//Forward declaration for accept method in Expr
struct Visitor;

struct Expr {
    virtual ~Expr() = default;
    // accept method is const since we won't make changes to the Expr object
    virtual void accept(Visitor& visitor) const = 0;
};


// Forward declarations we will need
struct AssignExpr;
struct LiteralExpr;
struct LogicalExpr;
struct GroupingExpr;
struct UnaryExpr;
struct BinaryExpr;
struct VariableExpr;
struct CallExpr;

//TODO: visitor should maybe return ValueType
struct Visitor {
    virtual void visit_assign_node(const AssignExpr& node) = 0;
    virtual void visit_literal_node(const LiteralExpr& node) = 0;
    virtual void visit_logical_node(const LogicalExpr& node) = 0;
    virtual void visit_grouping_node(const GroupingExpr& node) = 0;
    virtual void visit_unary_node(const UnaryExpr& node) = 0;
    virtual void visit_binary_node(const BinaryExpr& node) = 0;
    virtual void visit_variable_expr(const VariableExpr& node) = 0;
    virtual void visit_call_expr(const CallExpr& node) = 0;
};


struct AssignExpr: public Expr {
    Token name_;
    std::unique_ptr<Expr> value_;

    AssignExpr(Token name_, std::unique_ptr<Expr> value):
        name_{std::move(name_)}, value_{std::move(value)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_assign_node(*this);
    }
};

struct LiteralExpr: public Expr {
    ValueType literal_;

    template<typename LiteralType>
    explicit LiteralExpr(LiteralType&& literal):
        literal_{std::forward<LiteralType>(literal)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_literal_node(*this);
    }
};

struct LogicalExpr: public Expr {
    Token op_;
    std::unique_ptr<Expr> lhs_;
    std::unique_ptr<Expr> rhs_;

    LogicalExpr(Token op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs):
        op_{std::move(op)}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_logical_node(*this);
    }
};

struct GroupingExpr: public Expr {
    std::unique_ptr<Expr> expr_;

    explicit GroupingExpr(std::unique_ptr<Expr> expr):
        expr_{std::move(expr)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_grouping_node(*this);
    }
};

struct UnaryExpr: public Expr {
    Token op_;
    std::unique_ptr<Expr> expr_;

    UnaryExpr(Token op, std::unique_ptr<Expr> expr):
        op_{std::move(op)}, expr_{std::move(expr)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_unary_node(*this);
    }
};

struct BinaryExpr: public Expr {
    Token op_;
    std::unique_ptr<Expr> lhs_;
    std::unique_ptr<Expr> rhs_;

    BinaryExpr(Token op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs):
        op_{std::move(op)}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_binary_node(*this);
    }
};

struct VariableExpr: public Expr {
    Token name_;

    VariableExpr(Token name): name_{std::move(name)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_variable_expr(*this);
    }
};

struct CallExpr: public Expr {
    std::unique_ptr<Expr> callee_;
    Token paren_;                       // store this token for location
    std::vector<std::unique_ptr<Expr>> arguments_;

    CallExpr(std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments):
        callee_{std::move(callee)}, paren_{std::move(paren)}, arguments_{std::move(arguments)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_call_expr(*this);
    }
};

} // namespace lox
