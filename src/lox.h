#pragma once

#include <string>
#include <fmt/core.h>
#include "token.h"

namespace lox {
struct Lox {
    struct RuntimeError: public std::runtime_error {
        Token token_;

        template<typename StringType>
        RuntimeError(Token token, StringType&& message):
            std::runtime_error(std::forward<StringType>(message)),
            token_{std::move(token)} {}
    };

    inline static bool had_error = false;
    inline static bool had_runtime_error = false;

    static void error(Token token, std::string message) {
        if (token.type_ == TokenType::END) {
            report(token.line_, " at end", message);
        } else {
            report(token.line_, " at ", message);
        }
        had_error = true;
    }

    static void error(int line, const std::string& message) {
        report(line, "", message);
        had_error = true;
    }

    static void runtime_error(const RuntimeError& error) {
        fmt::print(stderr, "{}\n[line {}]\n", error.what(), error.token_.line_);
        had_runtime_error = true;
    }

    private:
    static void report(int line, const std::string& where, const std::string message) {
        fmt::print("[line {}] Error {}: {}\n", line, where, message);
    }
};
} //namespace lox
