#pragma once

#include <vector>

#include "Constants.hh"
#include "IrFunction.hh"

class IrProgram {
    MAKE_DEFAULT_CONSTRUCTABLE(IrProgram)
    MAKE_NONMOVABLE(IrProgram)
    MAKE_NONCOPYABLE(IrProgram)

   public:
    void add_function(FunctionDecl*);
    const IrFunction& get_function_from_id(unsigned id) const;

    void dump() const;

    inline int main_function_id() const { return m_main_function_id; }

    IrFunction& current_func() { return *m_functions.back(); }

   private:
    int m_main_function_id {};
    std::vector<ptr_t<IrFunction>> m_functions {};
};