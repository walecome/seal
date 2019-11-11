#pragma once

#include "Operand.hh"

class IntegerLiteral;

namespace Generate {

Operand integer_literal(IntegerLiteral *int_literal);

}