#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include <fmt/format.h>

#include "Constants.hh"
#include "Lexer.hh"
#include "Parser.hh"
#include "Scope.hh"
#include "TokenBuffer.hh"
#include "Util.hh"
#include "argparse.h"
#include "ast/FunctionDecl.hh"
#include "interpreter/Interpreter.hh"
#include "ir/Generate.hh"
#include "ir/IrProgram.hh"
#include "ir/QuadCollection.hh"

#include "interpreter/InstructionSequencer.hh"

#include "common/Relocator.hh"

#include "CrashHelper.hh"

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
    CrashHelper::the()->register_signals();
    ArgumentParser argument_parser = parse_args(argc, argv);

    std::string source_file { argument_parser.get<std::string>("source") };

    bool verbose { argument_parser.get<bool>("verbose") };

    if (!std::filesystem::exists(source_file)) {
        fmt::print("The input file \"{}\" doesn't exist\n", source_file);
        exit(1);
    }

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

    ptr_t<ConstantPool> constant_pool = std::make_unique<ConstantPool>();
    Generate ir_generator { parser.compilation_unit.get(),
                            constant_pool.get() };
    QuadCollection quads;
    long ir_duration = measure_time([&] { quads = ir_generator.generate(); });

    if (verbose) {
        std::cout << "IR generation took " << ir_duration << " milliseconds"
                  << std::endl;
        fmt::print("\n");

        quads.dump();
        fmt::print("Constant pool:\n");
        constant_pool->dump();
        fmt::print("\n");
    }

    std::vector<RelocatedQuad> relocated_quads =
        relocator::relocate_quads(quads);

    constexpr int width = 10 + 15 + 25*3 + 10;
    auto print_row = [](
                         const std::string_view a, const std::string_view b,
                         const std::string_view c, const std::string_view d,
                         const std::string e) {
        // -2 for '|' and space.
        fmt::print("|{:>{}} ", a, 10);
        fmt::print("|{:>{}} ", b, 15);
        fmt::print("|{:>{}} ", c, 25);
        fmt::print("|{:>{}} ", d, 25);
        fmt::print("|{:>{}} ", e, 25);
        fmt::print("|\n");
    };

    auto print_separator = [] { fmt::print("{:-^{}}\n", "", width + 1); };

    auto print_column_header = [print_row, print_separator] {
        print_separator();
        print_row("ADDRESS", "OPCODE", "DEST", "SRC_A", "SRC_B");
        print_separator();
    };

    print_column_header();

    for (size_t addr = 0; addr < relocated_quads.size(); ++addr) {
        const auto &quad = relocated_quads[addr];
        auto s = quad.stringify();
        std::string address = fmt::format("{0:#x}", addr);
        print_row(address, s.opcode, s.dest, s.src_a, s.src_b);
    }
    print_separator();

    return 1;

    ASSERT_NOT_REACHED_MSG("FIXME: Add resolvers");
    Interpreter interpreter { nullptr, nullptr, verbose };
    interpreter.interpret();

    return 0;
}
