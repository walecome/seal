#include "CTypeWrapper.hh"
#include <memory>
#include <string_view>
#include "Constants.hh"

namespace vm {

namespace {
class CIntWrapper : public CTypeWrapper {
   public:
    CIntWrapper(unsigned long value) : m_value(value) {}

    void* to_arg() const override { return reinterpret_cast<void*>(m_value); }

   private:
    const unsigned long m_value;
};

class CStringWrapper : public CTypeWrapper {
   public:
    CStringWrapper(std::string_view value) : m_value(std::string(value)) {}

    void* to_arg() const override {
        return const_cast<void*>(
            reinterpret_cast<const void*>(m_value.c_str()));
    }

   private:
    const std::string m_value;
};

}  // namespace

ptr_t<CTypeWrapper> CTypeWrapper::from(ValueOperand value_operand) {
    if (value_operand.is_integer()) {
        return std::make_unique<CIntWrapper>(value_operand.as_int());
    }
    if (value_operand.is_real()) {
        ASSERT_NOT_REACHED_MSG("C real type not supported yet");
    }
    if (value_operand.is_string()) {
        return std::make_unique<CStringWrapper>(value_operand.as_string());
    }
    if (value_operand.is_vector()) {
        ASSERT_NOT_REACHED_MSG("C vector type not supported yet");
    }

    ASSERT_NOT_REACHED();
}

}  // namespace vm