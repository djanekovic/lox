#pragma once

#include <memory>
#include "../token.h"
#include "stmt.h"

namespace lox {
struct Expr;
struct VarStmt: public Stmt {
    Token name_;
    std::unique_ptr<Expr> initializer_;

    VarStmt() = default;
    VarStmt(Token name, std::unique_ptr<Expr> initializer):
        name_{std::move(name)}, initializer_{std::move(initializer)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_var_stmt(*this);
    }
};

}
