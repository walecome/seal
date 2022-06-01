#pragma once

#include "interpreter/InstructionAddress.hh"
#include "ir/Operand.hh"

class LabelResolver {
  public:
    InstructionAddress resolve_label(LabelOperand label) const;
  private:
};

