#include <iostream>
#include <cassert>
#include <map>
#include <memory>
#include <functional>

#include <yaml-cpp/yaml.h>
#include "lox/stmt/stmt.h"
#include "lox/stmt/var_stmt.h"
#include "lox/token.h"
#include "lox/expr/expr.h"

namespace YAML {
const inline std::map<std::string_view, lox::TokenType> string_to_token_type_{
    {"LEFT_PAREN", lox::TokenType::LEFT_PAREN},
    {"RIGHT_PAREN", lox::TokenType::RIGHT_PAREN},
    {"LEFT_BRACE", lox::TokenType::LEFT_BRACE},
    {"RIGHT_BRACE", lox::TokenType::RIGHT_BRACE},
    {"COMMA", lox::TokenType::COMMA},
    {"DOT", lox::TokenType::DOT},
    {"MINUS", lox::TokenType::MINUS},
    {"PLUS", lox::TokenType::PLUS},
    {"SEMICOLON", lox::TokenType::SEMICOLON},
    {"SLASH", lox::TokenType::SLASH},
    {"STAR", lox::TokenType::STAR},
    {"BANG", lox::TokenType::BANG},
    {"BANG_EQUAL", lox::TokenType::BANG_EQUAL},
    {"EQUAL", lox::TokenType::EQUAL},
    {"EQUAL_EQUAL", lox::TokenType::EQUAL_EQUAL},
    {"GREATER", lox::TokenType::GREATER},
    {"GREATER_EQUAL", lox::TokenType::GREATER_EQUAL},
    {"LESS", lox::TokenType::LESS},
    {"LESS_EQUAL", lox::TokenType::LESS_EQUAL},
    {"IDENTIFIER", lox::TokenType::IDENTIFIER},
    {"STRING", lox::TokenType::STRING},
    {"NUMBER", lox::TokenType::NUMBER},
    {"AND", lox::TokenType::AND},
    {"CLASS", lox::TokenType::CLASS},
    {"ELSE", lox::TokenType::ELSE},
    {"FALSE", lox::TokenType::FALSE},
    {"FUN", lox::TokenType::FUN},
    {"FOR", lox::TokenType::FOR},
    {"IF", lox::TokenType::IF},
    {"NIL", lox::TokenType::NIL},
    {"OR", lox::TokenType::OR},
    {"PRINT", lox::TokenType::PRINT},
    {"RETURN", lox::TokenType::RETURN},
    {"SUPER", lox::TokenType::SUPER},
    {"THIS", lox::TokenType::THIS},
    {"TRUE", lox::TokenType::TRUE},
    {"VAR", lox::TokenType::VAR},
    {"WHILE", lox::TokenType::WHILE},
    {"END", lox::TokenType::END}
};

template<>
struct convert<lox::TokenType> {
    static bool decode(const Node& node, lox::TokenType& token_type) {
        if (!node.IsScalar()) return false;

        const auto it = string_to_token_type_.find(node.Scalar());
        if (it == std::cend(string_to_token_type_)) return false;

        token_type = it->second;
        return true;
    }
};

template<>
struct convert<lox::Token> {
    static bool decode(const Node& node, lox::Token& token) {
        if (!node.IsMap()) return false;

        if (!node["type"]) return false;
        token.type_ = node["type"].as<lox::TokenType>();

        switch (token.type_) {
            case lox::TokenType::IDENTIFIER:
            case lox::TokenType::STRING:
                if (!node["lexeme"]) return false;
                token.lexeme_ = node["lexeme"].as<std::string>();
                break;
            case lox::TokenType::NUMBER:
                if (!node["lexeme"]) return false;
                token.lexeme_ = node["lexeme"].as<double>();
                break;
            default:
                token.lexeme_ = std::monostate();
        }

        if (!node["line"]) return false;
        token.line_ = node["line"].as<int>();

        return true;
    }
};

template<>
struct convert<std::unique_ptr<lox::Expr>> {
    static bool decode(const Node& node, std::unique_ptr<lox::Expr>& expr) {
        return true;
    }
};

template<>
struct convert<lox::VarStmt> {
    static bool decode(const Node& node, lox::VarStmt& stmt) {
        if (!node.IsMap()) return false;

        if (!node["name"]) return false;
        auto name = node["name"].as<lox::Token>();

        if (!node["initializer"]) return false;
        auto initializer = node["initializer"].as<std::unique_ptr<lox::Expr>>();

        return true;
    }
};

template<>
struct convert<std::unique_ptr<lox::Stmt>> {
    static bool decode(const Node& node, std::unique_ptr<lox::Stmt>& stmt) {
        if (!node.IsMap()) return false;

        auto stmt_type = node.Scalar();

        if (stmt_type == "VarStmt") {
            stmt = std::make_unique<lox::VarStmt>(node[stmt_type].as<lox::VarStmt>());
        }
        return true;
    }
};

template<>
struct convert<std::vector<std::unique_ptr<lox::Stmt>>> {
    static bool decode(const Node& node, std::vector<std::unique_ptr<lox::Stmt>>& stmts) {
        if(!node.IsSequence()) return false;

        stmts.reserve(node.size());
        for (int i = 0; i < node.size(); i++) {
            stmts.push_back(node[0].as<std::unique_ptr<lox::Stmt>>());
        }

//        std::transform(std::cbegin(node), std::cend(node), std::back_inserter(stmts),
//                       [](const auto& n) { return n.template as<std::unique_ptr<lox::Stmt>>(); });
        return true;
    }
};
} // namespace YAML
