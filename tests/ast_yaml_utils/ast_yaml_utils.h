#pragma once

#include <string>
#include <vector>
#include <memory>

#include "lox/stmt/stmt.h"

namespace test_utils {
std::vector<std::unique_ptr<lox::Stmt>> generateASTFromYaml(std::string&& yaml_document);
} //namespace test_utils
