#pragma once

#include <vector>

#include "Constants.hh"
#include "Quad.hh"

class IrFunction {
    MAKE_DEFAULT_CONSTRUCTABLE(IrFunction)
    MAKE_NONCOPYABLE(IrFunction)
    MAKE_NONMOVABLE(IrFunction)

   public:
    void add_quad(ptr_t<Quad>&& quad);
    void dump_quads() const;

   private:
    std::vector<ptr_t<Quad>> m_quads {};
};