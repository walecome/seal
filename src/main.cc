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

ArgumentParser parse_args(int argc, char **argv) {
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
long measure_time(Function f) {
    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    f();
    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();

    return duration;
}

int main(int argc, char **argv) {
    ArgumentParser argument_parser = parse_args(argc, argv);

    std::string source_file { argument_parser.get<std::string>("source") };

    bool verbose { argument_parser.get<bool>("verbose") };

    sptr_t<std::string> source = util::read_source(source_file);
    Lexer lexer { *source };

    long lexer_duration = measure_time([&]() { lexer.read_all(); });

    if (verbose) lexer.get_tokens().print_tokens();

    std::cout << "Lexed " << lexer.number_of_tokens() << " tokens in "
              << lexer_duration << " milliseconds" << std::endl;

    Parser parser {};

    long parser_duration =
        measure_time([&]() { parser.parse(lexer.get_tokens()); });

    std::cout << "Parsing took " << parser_duration << " milliseconds"
              << std::endl;

    if (verbose) std::cout << parser.compilation_unit->dump() << std::endl;

    sptr_t<Scope> program_scope = std::make_shared<Scope>();

    parser.compilation_unit->function_pass(program_scope.get());
    std::cout << "Function pass got " << program_scope->function_count()
              << " functions" << std::endl;

    return 0;
}