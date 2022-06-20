#pragma once

#include <vector>

#include "Constants.hh"
#include "IrFunction.hh"

class IrProgram {
    MAKE_DEFAULT_CONSTRUCTABLE(IrProgram)
    MAKE_NONMOVABLE(IrProgram)
    MAKE_NONCOPYABLE(IrProgram)

   public:
    void add_function(ptr_t<IrFunction>& function);

    void dump() const;

    inline int main_function_id() const { return m_main_function_id; }

   private:
    int m_main_function_id {};
    std::vector<ptr_t<IrFunction>> m_functions {};
};
