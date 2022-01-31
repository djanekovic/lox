#pragma once

#include <memory>
#include <vector>
#include "stmt.h"

namespace lox {
struct BlockStmt: public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements_;

    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>>&& statements):
        statements_{std::move(statements)} {}

    void accept(StmtVisitor& visitor) const override {
        return visitor.visit_block_stmt(*this);
    }
};
}
