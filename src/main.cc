#include <chrono>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include "Constants.hh"
#include "Lexer.hh"
#include "Parser.hh"
#include "Scope.hh"
#include "TokenBuffer.hh"
#include "Util.hh"
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
    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    f();
    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();

    return duration;
}

int main(int argc, char **argv) {
    ArgumentParser argumentParser = parseArgs(argc, argv);

    std::string source_file { argumentParser.get<std::string>("source") };

    bool verbose { argumentParser.get<bool>("verbose") };

    sptr_t<std::string> source = Util::readSource(source_file);
    Lexer lexer { *source };

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

    sptr_t<Scope> programScope = std::make_shared<Scope>();

    parser.compilationUnit->functionPass(programScope.get());
    std::cout << "Function pass got " << programScope->functionCount()
              << " functions" << std::endl;

    return 0;
}