#include <chrono>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "Lexer.hh"
#include "Parser.hh"
#include "Scope.hh"
#include "TokenBuffer.hh"
#include "argparse.h"

ArgumentParser parseArgs(int argc, char **argv) {
    ArgumentParser parser("CLI argument parser");
    parser.add_argument("--source", "The filename of the source file", true);
    parser.add_argument("--verbose", "Use verbose compiling", false);
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

void eatAllTokens(TokenBuffer &tokenBuffer) {
    while (!tokenBuffer.empty()) {
        std::cout << "Ate token with value: " << tokenBuffer.pop().value
                  << std::endl;
    }
}

int main(int argc, char **argv) {
    ArgumentParser argumentParser = parseArgs(argc, argv);

    std::string source_file { argumentParser.get<std::string>("source") };

    std::ifstream t(source_file);
    std::string code_text((std::istreambuf_iterator<char>(t)),
                          std::istreambuf_iterator<char>());

    bool verbose { argumentParser.get<bool>("verbose") };

    Lexer lexer { code_text };

    long lexerDuration = measureTime([&]() { lexer.readAll(); });

    if (verbose) lexer.getTokens().printTokens();

    std::cout << "Lexed " << lexer.numberOfTokens() << " tokens in "
              << lexerDuration << " milliseconds" << std::endl;

    Parser parser {};

    long parserDuration =
        measureTime([&]() { parser.parse(lexer.getTokens()); });

    std::cout << "Parsing took " << parserDuration << " milliseconds"
              << std::endl;

    if (verbose) std::cout << parser.compilationUnit->dump() << std::endl;

    Scope programScope {};
    parser.compilationUnit->functionPass(programScope);
    std::cout << "Function pass got " << programScope.functionCount()
              << " functions" << std::endl;

    return 0;
}