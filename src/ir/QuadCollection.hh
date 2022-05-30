#pragma once

#include <map>
#include <vector>
#include <string>

#include "common/ValuePool.hh"
#include "ir/Quad.hh"

struct QuadCollection {
    std::map<unsigned, unsigned> function_to_quad {};
    std::map<unsigned, std::string> function_to_string{};
    std::map<unsigned, unsigned> label_to_quad {};
    std::vector<Quad> quads {};
    unsigned main_function_id {};

    unsigned register_count;

    void dump() const;
};
