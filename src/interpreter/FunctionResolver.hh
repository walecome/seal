#pragma once

#include "interpreter/InstructionAddress.hh"
#include "ir/Operand.hh"

class FunctionResolver {
  public:
    InstructionAddress resolve_function(FunctionOperand function) const;
  private:
};



