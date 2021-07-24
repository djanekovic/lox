#include <iostream>
#include <memory>
#include "scanner.h"
#include "lox.h"


void run(std::string command)
{
    lox::Scanner scanner(std::move(command));

    auto tokens = scanner.scan_tokens();

    for (auto &token : tokens) {
        std::cout << token.to_string() << std::endl;
    }
}


int run_prompt()
{
    for (;;) {
        std::string command;

        std::cout << "> " << std::flush;

        // ^D to exit
        std::getline(std::cin, command);
        if (command.empty()) {
            break;
        }

        run(std::move(command));

        if (lox::Lox::had_error) {
            break;
        }
    }

    return lox::Lox::had_error ? 65 : 0;
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
        std::cerr << "Failed reading file " << filename << std::endl;
        return 1;
    }

    run(std::move(filecontents));

    return lox::Lox::had_error ? 65 : 0;
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [script]" << std::endl;
        return 1;
    } else if (argc == 1) {
        return run_prompt();
    } else {
        return run_file(argv[1]);
    }
}
