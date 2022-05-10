#pragma once

#include "ffi.h"

#include "Constants.hh"
#include "interpreter/Value.hh"
#include "utility/StringTable.hh"

namespace vm {
class CTypeWrapper {
   public:
    static ptr_t<CTypeWrapper> from(const StringTable* string_table, Value value);
    virtual ~CTypeWrapper() = default;

    ffi_type& get_type() {
        return m_type;
    }
    virtual void* get_value() = 0;

    protected:
        CTypeWrapper(ffi_type type) : m_type(type) {}


    ffi_type m_type;
};

class CResultWrapper {
public:
    CResultWrapper() = default;
    CResultWrapper(ffi_type type) : m_type(type) {
        m_buffer = new ffi_arg;
    }

    ~CResultWrapper() {
        delete m_buffer;
    }

    ffi_type& type() {
        return m_type;
    }

    ffi_arg* buffer() {
        return m_buffer;
    }

    private:

    ffi_type m_type;
    ffi_arg* m_buffer;
};

}  // namespace vm
