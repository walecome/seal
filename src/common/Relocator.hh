#pragma once

#include <vector>

#include "common/RelocatedQuad.hh"
#include "ir/QuadCollection.hh"

namespace relocator {
std::vector<RelocatedQuad> relocate_quads(
    const QuadCollection& quad_collection);
}
