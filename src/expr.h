#pragma once
#include <memory>

//Forward declaration for accept method in Expr
class Visitor;

struct Expr {
    virtual ~Expr() = default;
    // accept method is const since we won't make changes to the Expr object
    virtual void accept(Visitor& visitor) const = 0;
};


// Forward declarations we will need
class LiteralExpr;
class GroupingExpr;
class UnaryExpr;
class BinaryExpr;

struct Visitor {
    virtual void visit_literal_node(const LiteralExpr& node) = 0;
    virtual void visit_grouping_node(const GroupingExpr& node) = 0;
    virtual void visit_unary_node(const UnaryExpr& node) = 0;
    virtual void visit_binary_node(const BinaryExpr& node) = 0;
};

struct LiteralExpr: public Expr {
    const std::string literal_;

    explicit LiteralExpr(std::string literal):
        literal_{std::move(literal)} {}

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

enum class UnaryOperator {
    BANG, MINUS
};

struct UnaryExpr: public Expr {
    const UnaryOperator op_;
    const std::unique_ptr<Expr> expr_;

    UnaryExpr(const UnaryOperator op, std::unique_ptr<Expr> expr):
        op_{op}, expr_{std::move(expr)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_unary_node(*this);
    }
};

enum class BinaryOperator {
    EQUAL_EQUAL, BANG_EQUAL,
    LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    PLUS, MINUS, STAR, SLASH,
    AND, OR
};

struct BinaryExpr: public Expr {
    const BinaryOperator op_;
    const std::unique_ptr<Expr> lhs_;
    const std::unique_ptr<Expr> rhs_;

    BinaryExpr(const BinaryOperator op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs):
        op_{op}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

    void accept(Visitor& visitor) const override {
        visitor.visit_binary_node(*this);
    }
};

class ASTPrettyPrinter: public Visitor {
    std::string format;

public:
    ASTPrettyPrinter() = default;

    void visit_literal_node(const LiteralExpr& node) override;
    void visit_grouping_node(const GroupingExpr& node) override;
    void visit_unary_node(const UnaryExpr& node) override;
    void visit_binary_node(const BinaryExpr& node) override;

    std::string to_string() const { return format; }
};
