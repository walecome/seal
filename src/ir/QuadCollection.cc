#include <fmt/format.h>

#include "ir/QuadCollection.hh"

void QuadCollection::dump() const {
    constexpr int width = 120;

    auto print_row = [](const std::string_view a, const std::string_view b,
                        const std::string_view c, const std::string_view d,
                        const std::string e, const std::string f) {
        // -2 for '|' and space.
        int col_width = 120 / 6 - 2;
        fmt::print("|{:>{}} ", a, col_width);
        fmt::print("|{:>{}} ", b, col_width);
        fmt::print("|{:>{}} ", c, col_width);
        fmt::print("|{:>{}} ", d, col_width);
        fmt::print("|{:>{}} ", e, col_width);
        fmt::print("|{:>{}} ", f, col_width);
        fmt::print("|\n");
    };

    auto print_separator = [] { fmt::print("{:-^{}}\n", "", width + 1); };

    auto print_column_header = [print_row, print_separator] {
        print_separator();
        print_row("ADDRESS", "META", "OPCODE", "DEST", "SRC_A", "SRC_B");
        print_separator();
    };

    auto print_function_header = [print_column_header, print_separator](
                                     auto function_id, auto function_name,
                                     bool first) {
        if (!first) {
            print_separator();
            fmt::print("\n");
        }
        print_separator();
        std::string content =
            fmt::format("\"{}\" (Function #{})", function_name, function_id);
        fmt::print("|{:^{}}|\n", content, width - 1);
        print_column_header();
    };

    std::map<unsigned, unsigned> quad_to_function {};

    for (auto [function_id, quad_idx] : function_to_quad) {
        quad_to_function.insert({ quad_idx, function_id });
    }

    bool first_function_printed = false;
    for (unsigned i = 0; i < quads.size(); ++i) {
        auto [labels, opcode, dest, src_a, src_b] =
            quads[i].to_string_segments();

        bool new_function = quad_to_function.find(i) != quad_to_function.end();
        if (new_function) {
            auto function_id = quad_to_function[i];
            print_function_header(function_id,
                                  function_to_string.at(function_id),
                                  !first_function_printed);
            first_function_printed = true;
        }

        std::string address = fmt::format("{0:#x}", i);
        print_row(address, labels, opcode, dest, src_a, src_b);
    }
    print_separator();
}
