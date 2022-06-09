#pragma once

#include <map>

#include "interpreter/InstructionAddress.hh"
#include "ir/Operand.hh"

class FunctionResolver {
   public:
    void add_function(FunctionOperand function, InstructionAddress addr);
    InstructionAddress resolve_function(FunctionOperand function) const;

   private:
    std::map<FunctionOperand, InstructionAddress> m_address_map {};
};
