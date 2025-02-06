#include "gtest/gtest.h"

#include "lox/scanner.h"
#include "lox/parser.h"

namespace {
auto parse(std::string&& program) {
    auto tokens = lox::scan_tokens(std::move(program));
    lox::Parser parser(std::move(tokens));
    return parser.parse();
}
}