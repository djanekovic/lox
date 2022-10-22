#pragma once

#include "../token.h"
#include "stmt.h"

namespace lox {
struct ClassStmt : public Stmt {
    Token name_;
    std::vector<std::unique_ptr<FunctionStmt>> methods_;

    ClassStmt(Token&& name, std::vector<std::unique_ptr<FunctionStmt>>&& methods):
        name_{std::move(name)}, methods_{std::move(methods)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_class_stmt(*this);
    }
};
}
