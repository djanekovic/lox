#include "gtest/gtest.h"

#include "lox/scanner.h"
#include "lox/parser.h"

#include "ast_yaml_utils/ast_yaml_utils.h"

namespace {
auto parse(std::string&& program) {
    lox::Scanner scanner(std::move(program));
    auto tokens = scanner.scan_tokens();
    lox::Parser parser(std::move(tokens));
    return parser.parse();
}
}

TEST(ParserTest, VariableExpr) {
    auto test_str = R"(var breakfast = "bagels")";
    auto correct_ast_json = R"(
ast:
  - VarStmt:
    name:
      type: "IDENTIFIER"
      lexeme: "breakfast"
      line: 1
    initializer:
      AssignExpr:
        name:
          type: "STRING"
          lexeme: "bagels"
          line: 1
)";
    const auto ast = test_utils::generateASTFromYaml(correct_ast_json);
}


TEST(ParserTest, Variables) {
    auto test_str = R"(
var breakfast = "bagels";
print breakfast;
breakfast = "beignets";
)";

    parse(std::move(test_str));
}
