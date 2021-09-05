#pragma once
#include "Constants.hh"
#include "Operand.hh"

namespace vm {
class CTypeWrapper {
   public:
    static ptr_t<CTypeWrapper> from(ValueOperand value_operand);
    virtual ~CTypeWrapper() = default;
    
    virtual void* to_arg() const = 0;
};
}  // namespace vm