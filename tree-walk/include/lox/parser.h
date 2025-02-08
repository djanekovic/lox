#pragma once

#include <memory>
#include <vector>

#include "token.h"
#include "stmt/stmt.h"

namespace lox {
std::vector<std::unique_ptr<Stmt>> parse(std::vector<Token>&& tokens);
} //namespace lox
