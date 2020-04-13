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

    for (const Quad &quad : quads) {
        auto [labels, opcode, dest, src_a, src_b] = quad.to_string_segments();
        print_row(labels, opcode, dest, src_a, src_b);
    }
    fmt::print("{:-^{}}\n", "", width);
}
