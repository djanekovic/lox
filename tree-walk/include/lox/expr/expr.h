#pragma once
namespace lox {
//Forward declaration for accept method in Expr
struct ExprVisitor;

struct Expr {
    virtual ~Expr() = default;
    // accept method is const since we won't make changes to the Expr object
    virtual void accept(ExprVisitor& visitor) const = 0;
};

// Forward declarations we will need
struct AssignExpr;
struct BinaryExpr;
struct CallExpr;
struct GetExpr;
struct SetExpr;
struct GroupingExpr;
struct LiteralExpr;
struct LogicalExpr;
struct UnaryExpr;
struct VariableExpr;

//TODO: visitor should maybe return ValueType
struct ExprVisitor {
    virtual void visit_assign_node(const AssignExpr& node) = 0;
    virtual void visit_binary_node(const BinaryExpr& node) = 0;
    virtual void visit_call_expr(const CallExpr& node) = 0;
    virtual void visit_get_expr(const GetExpr& node) = 0;
    virtual void visit_set_node(const SetExpr& node) = 0;
    virtual void visit_grouping_node(const GroupingExpr& node) = 0;
    virtual void visit_literal_node(const LiteralExpr& node) = 0;
    virtual void visit_logical_node(const LogicalExpr& node) = 0;
    virtual void visit_unary_node(const UnaryExpr& node) = 0;
    virtual void visit_variable_expr(const VariableExpr& node) = 0;
};




} // namespace lox
