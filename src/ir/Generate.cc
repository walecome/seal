#include "Generate.hh"
#include "OperandType.hh"
#include "ast/IntegerLiteral.hh"

Operand Generate::integer_literal(IntegerLiteral *integer_literal) {
    OperandType type { ValueKind::SIGNED, 4 };

    return Operand::create_immediate(std::move(type), integer_literal->value());
}