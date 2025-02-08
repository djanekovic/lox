#pragma once
#include <string>
#include <vector>
#include "token.h"

namespace lox {
std::vector<Token> scan_tokens(std::string&& program);
} // namespace lox
