#pragma once

#include <map>

#include "interpreter/InstructionAddress.hh"
#include "ir/IrOperand.hh"

class LabelResolver {
   public:
    void add_label(LabelOperand label, InstructionAddress addr);
    InstructionAddress resolve_label(LabelOperand label) const;

   private:
    std::map<LabelOperand, InstructionAddress> m_address_map {};
};
