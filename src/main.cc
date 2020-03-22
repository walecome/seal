#include <chrono>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
// #include <variant>

#include <fmt/format.h>

#include "Constants.hh"
// #include "Interpreter.hh"
#include "Lexer.hh"
#include "Parser.hh"
#include "Scope.hh"
#include "TokenBuffer.hh"
#include "Util.hh"
#include "argparse.h"
#include "ast/FunctionDecl.hh"
// #include "interpreter/Interpreter.hh"
#include "ir/Generate.hh"
#include "ir/IrProgram.hh"

ArgumentParser parse_args(int argc, char **argv) {
    ArgumentParser parser("CLI argument parser");
    parser.add_argument("--source", "The filename of the source file", true);
    parser.add_argument("--verbose", "Use verbose compiling", false);
    parser.add_argument("--interpret", "Interpret instead of compiling", false);

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

    long lexer_duration = measure_time([&] { lexer.read_all(); });

    if (verbose) {
        lexer.get_tokens().print_tokens();
        std::cout << "Lexed " << lexer.number_of_tokens() << " tokens in "
                  << lexer_duration << " milliseconds" << std::endl;
    }
    Parser parser {};

    long parser_duration =
        measure_time([&] { parser.parse(lexer.get_tokens()); });

    if (verbose) {
        std::cout << "Parsing took " << parser_duration << " milliseconds"
                  << std::endl;

        std::cout << parser.compilation_unit->dump() << std::endl;
    }

    ptr_t<Scope> program_scope = std::make_unique<Scope>();

    parser.compilation_unit->function_pass(program_scope.get());
    if (verbose) {
        std::cout << "Function pass got " << program_scope->function_count()
                  << " functions" << std::endl;
    }

    long semantic_duration = measure_time(
        [&] { parser.compilation_unit->analyze(program_scope.get()); });
    error::report_semantic_errors();

    if (verbose) {
        std::cout << "Semantic analysis took " << semantic_duration
                  << " milliseconds" << std::endl;
        std::cout << "AST after semantic analysis:" << std::endl;
        std::cout << parser.compilation_unit->dump() << std::endl;
    }

    Generate ir_generator { parser.compilation_unit.get() };
    ptr_t<IrProgram> ir_program;
    long ir_duration =
        measure_time([&] { ir_program = ir_generator.generate(); });

    if (verbose) {
        std::cout << "IR generation took " << ir_duration << " milliseconds"
                  << std::endl;
        ir_program->dump();
    }

    // Interpreter interpreter { ir_program.get() };
    // interpreter.interpret();

    return 0;
}
