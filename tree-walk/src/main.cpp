#include <iostream>
#include <memory>
#include <fmt/core.h>

#include "lox/scanner.h"
#include "lox/parser.h"
#include "lox/resolver.h"
#include "lox/ast_pretty_printer.h"
#include "lox/interpreter.h"
#include "lox/lox.h"

namespace {
// we want to reuse this object
static lox::Interpreter interpreter;
} // anonymous namespace

void run(std::string&& command)
{
    auto tokens = lox::scan_tokens(std::move(command));
    //std::for_each(tokens.cbegin(), tokens.cend(), [](const auto& t) { fmt::print("{}\n", t.stringify_token()); });

    auto statements = lox::parse(std::move(tokens));

#if 0
    lox::ASTPrettyPrinter printer;
    expression->accept(printer);

    fmt::print("{}\n", printer.to_string());
#endif

    lox::Resolver resolver(interpreter);
    resolver.resolve(statements);

    if (lox::Lox::had_error) {
        return;
    }


    interpreter.interpret(std::move(statements));
}


int run_prompt()
{
    for (;;) {
        fmt::print("> ");

        // ^D to exit
        std::string command;
        std::getline(std::cin, command);
        if (command.empty()) {
            break;
        }

        run(std::move(command));

        if (lox::Lox::had_error) {
            return 65;
        }
    }

    return 0;
}

int run_file(const char *filename)
{
    std::string filecontents;
    std::unique_ptr<FILE, decltype(&std::fclose)> f{std::fopen(filename, "r"), &std::fclose};
    if (f) {
        std::fseek(f.get(), 0, SEEK_END);
        filecontents.assign(std::ftell(f.get()), 0);
        std::fseek(f.get(), 0, SEEK_SET);
        std::fread(filecontents.data(), sizeof(char), filecontents.size(), f.get());
    } else {
        fmt::print(stderr, "Failed reading file {}\n", filename);
        return 1;
    }

    run(std::move(filecontents));

    if (lox::Lox::had_error) {
        return 65;
    }

    if (lox::Lox::had_runtime_error) {
        return 70;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        // more than one arg
        fmt::print(stderr, "Usage {} [script]\n", argv[0]);
        return 1;
    } else if (argc == 1) {
        // no args
        return run_prompt();
    } else {
        // we have one arg
        return run_file(argv[1]);
    }
}
