#pragma once

#include <string>
#include <fmt/core.h>

namespace lox {
struct Lox {
    inline static bool had_error = false;

    static void error(int line, const std::string& message) {
        report(line, "", message);
    }

    private:
    static void report(int line, const std::string& where, const std::string message) {
        fmt::print("[line {}] Error {}: {}", line, where, message);
    }
};
} //namespace lox
