#include <chrono>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "Lexer.hh"
#include "argparse.h"


ArgumentParser parseArgs(int argc, char **argv) {
    ArgumentParser parser("CLI argument parser");
    parser.add_argument("--source", "The filename of the source file", true);
    try {
        parser.parse(argc, argv);
    } catch (const ArgumentParser::ArgumentNotFound &ex) {
        std::cerr << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (parser.is_help()) exit(EXIT_SUCCESS);

    return parser;
}

template <typename Function>
long measureTime(Function f) {
    auto t1 = std::chrono::high_resolution_clock::now();
    f();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    return duration;
}

int main(int argc, char **argv) {
    ArgumentParser parser = parseArgs(argc, argv);

    std::string source_file { parser.get<std::string>("source") };

    std::ifstream t(source_file);
    std::string code_text((std::istreambuf_iterator<char>(t)),
                          std::istreambuf_iterator<char>());

    Lexer lexer { code_text };

    long duration = measureTime([&] () {
        lexer.readAll();
    });

    std::cout << "Lexed " << lexer.numberOfTokens() << " tokens in "
              << duration << " milliseconds" << std::endl;

    return 0;
}