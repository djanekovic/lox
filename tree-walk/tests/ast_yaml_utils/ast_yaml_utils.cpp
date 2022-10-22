#include <yaml-cpp/yaml.h>

#include "convert.h"
#include "ast_yaml_utils.h"

namespace test_utils {
std::vector<std::unique_ptr<lox::Stmt>> generateASTFromYaml(std::string&& yaml_document) {
    auto root = YAML::Load(yaml_document);
    return root["ast"].as<std::vector<std::unique_ptr<lox::Stmt>>>();
}
}
