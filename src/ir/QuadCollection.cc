#include <fmt/format.h>

#include "ir/QuadCollection.hh"

void QuadCollection::dump() const {
    constexpr int width = 80;

    constexpr int col_width = 16;

    auto print_row = [col_width](
                         const std::string_view a, const std::string_view b,
                         const std::string_view c, const std::string_view d,
                         const std::string e) {
        fmt::print("|{:<{}}", a, col_width - 6);
        fmt::print("|{:<{}}", b, col_width);
        fmt::print("|{:<{}}", c, col_width);
        fmt::print("|{:<{}}", d, col_width);
        fmt::print("|{:<{}}", e, col_width);
        fmt::print("|\n");
    };

    fmt::print("{:-^{}}\n", "", width);
    print_row("ATTRIBUTES", "OPCODE", "DEST", "SRC_A", "SRC_B");
    fmt::print("{:-^{}}\n", "", width);

    std::map<unsigned, unsigned> quad_to_function {};

    for (auto [function_id, quad_idx] : function_to_quad) {
        quad_to_function.insert({ quad_idx, function_id });
    }

    for (unsigned i = 0; i < quads.size(); ++i) {
        auto [labels, opcode, dest, src_a, src_b] =
            quads[i].to_string_segments();

        if (quad_to_function.count(i)) {
            labels = fmt::format("F#{} {}", quad_to_function[i], labels);
        }
        print_row(labels, opcode, dest, src_a, src_b);
    }
    fmt::print("{:-^{}}\n", "", width);
}
