#include "common/Relocator.hh"

#include "fmt/format.h"

#include "builtin/BuiltIn.hh"

namespace {

bool needs_relocation(const IrOperand &operand) {
    return operand.is_function() || operand.is_label();
};

Operand convert_operand_without_relocation(const IrOperand &operand) {
    ASSERT(!needs_relocation(operand));
    if (!operand.is_used()) {
        return Operand::empty();
    }
    if (operand.is_constant_entry()) {
        return Operand(operand.as_constant_entry());
    }
    if (operand.is_register()) {
        return Operand(operand.as_register());
    }
    if (operand.is_builtin_function()) {
        return Operand(
            BuiltinFunctionAddress(operand.as_builtin_function().value));
    }
    ASSERT_NOT_REACHED();
}

Operand relocate_operand(const IrOperand &operand,
                         const QuadCollection &quad_collection) {
    ASSERT(needs_relocation(operand));

    if (operand.is_function()) {
        auto it =
            quad_collection.function_to_quad.find(operand.as_function().value);
        ASSERT(it != quad_collection.function_to_quad.end());
        return Operand(InstructionAddress(it->second));
    }
    if (operand.is_label()) {
        auto it = quad_collection.label_to_quad.find(operand.as_label().value);
        ASSERT(it != quad_collection.label_to_quad.end());
        return Operand(InstructionAddress(it->second));
    }
    ASSERT_NOT_REACHED();
}

Operand maybe_relocate_operand(const IrOperand &operand,
                               const QuadCollection &quad_collection) {
    if (!needs_relocation(operand)) {
        return convert_operand_without_relocation(operand);
    } else {
        return relocate_operand(operand, quad_collection);
    }
}

RelocatedQuad relocate_quad(const Quad &quad,
                            const QuadCollection &quad_collection) {
    auto relocate = [&quad_collection](const IrOperand &operand) {
        return maybe_relocate_operand(operand, quad_collection);
    };
    return RelocatedQuad(quad.opcode(), relocate(quad.dest()),
                         relocate(quad.src_a()), relocate(quad.src_b()));
}

}  // namespace

namespace relocator {

std::vector<RelocatedQuad> relocate_quads(
    const QuadCollection &quad_collection) {
    std::vector<RelocatedQuad> relocated_quads {};
    relocated_quads.reserve(quad_collection.quads.size());
    std::transform(quad_collection.quads.begin(), quad_collection.quads.end(),
                   std::back_inserter(relocated_quads),
                   [&quad_collection](const Quad &quad) {
                       return relocate_quad(quad, quad_collection);
                   });
    return relocated_quads;
}

}  // namespace relocator
