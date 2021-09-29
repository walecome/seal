#include <type_traits>

#include "CTypeWrapper.hh"
#include <memory>
#include <string_view>
#include "Constants.hh"

namespace vm {

namespace {

class CIntWrapper : public CTypeWrapper {
   public:

    explicit CIntWrapper(unsigned long value) : CTypeWrapper(ffi_type_uint64), m_value(value) {}

    void* get_value() override {
        return &m_value;
    }

   private:
     unsigned long m_value;
};

class CStringWrapper : public CTypeWrapper {
   public:
    explicit CStringWrapper(const StringTable* string_table, const StringTable::Key key)
        : CTypeWrapper(ffi_type_pointer), m_value(string_table->get_at(key)->c_str()) {}

    void* get_value() {
        return &m_value;
    }

   private:
    const char* m_value;
};

class CRealWrapper : public CTypeWrapper {
    public:
    explicit CRealWrapper(double value) : CTypeWrapper(ffi_type_double), m_value(value) {}

    void* get_value() override {
        return &m_value;
    }

    private:
        double m_value;
};

}  // namespace

ptr_t<CTypeWrapper> CTypeWrapper::from(const StringTable* string_table, ValueOperand value_operand) {
    if (value_operand.is_integer()) {
        return std::make_unique<CIntWrapper>(value_operand.as_int());
    }
    if (value_operand.is_real()) {
        return std::make_unique<CRealWrapper>(value_operand.as_real());
    }
    if (value_operand.is_string()) {
        return std::make_unique<CStringWrapper>(string_table, value_operand.as_string());
    }
    if (value_operand.is_vector()) {
        ASSERT_NOT_REACHED_MSG("C vector type not supported yet");
    }

    ASSERT_NOT_REACHED();
}

}  // namespace vm
