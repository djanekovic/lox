#pragma once
#include <variant>
#include <memory>
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
struct LiteralExpr;
struct GroupingExpr;
struct UnaryExpr;
struct BinaryExpr;

struct Visitor {
    virtual void visit_literal_node(const LiteralExpr& node) = 0;
    virtual void visit_grouping_node(const GroupingExpr& node) = 0;
    virtual void visit_unary_node(const UnaryExpr& node) = 0;
    virtual void visit_binary_node(const BinaryExpr& node) = 0;
};


struct LiteralExpr: public Expr {
    using LiteralType = std::variant<std::monostate, double, std::string, bool>;
    LiteralType literal_;

    template<typename LiteralType>
    explicit LiteralExpr(LiteralType&& literal):
        literal_{std::forward<LiteralType>(literal)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_literal_node(*this);
    }
};

struct GroupingExpr: public Expr {
    const std::unique_ptr<Expr> expr_;

    explicit GroupingExpr(std::unique_ptr<Expr> expr):
        expr_{std::move(expr)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_grouping_node(*this);
    }
};

struct UnaryExpr: public Expr {
    const Token op_;
    const std::unique_ptr<Expr> expr_;

    UnaryExpr(const Token op, std::unique_ptr<Expr> expr):
        op_{std::move(op)}, expr_{std::move(expr)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_unary_node(*this);
    }
};

struct BinaryExpr: public Expr {
    const Token op_;
    const std::unique_ptr<Expr> lhs_;
    const std::unique_ptr<Expr> rhs_;

    BinaryExpr(Token op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs):
        op_{std::move(op)}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_binary_node(*this);
    }
};
} // namespace lox
