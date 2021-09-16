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
    explicit CStringWrapper(std::string_view value) : CTypeWrapper(ffi_type_pointer), m_value_buffer(alloc_string(value)) {
        m_value = m_value_buffer.get();
        ASSERT(m_value == value);
    }

    void* get_value() override {
        return &m_value;
    }

   private:
    std::unique_ptr<char[]> alloc_string(std::string_view input) {
        auto ptr = std::unique_ptr<char[]>(new char[input.size() + 1]);
        for (std::size_t i = 0; i < input.size(); ++i) {
            ptr.get()[i] = std::move(input[i]);
        }
        ptr.get()[input.size()] = '\0';
        return ptr;
    }

    std::unique_ptr<char[]> m_value_buffer;
    char* m_value;
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

ptr_t<CTypeWrapper> CTypeWrapper::from(ValueOperand value_operand) {
    if (value_operand.is_integer()) {
        return std::make_unique<CIntWrapper>(value_operand.as_int());
    }
    if (value_operand.is_real()) {
        return std::make_unique<CRealWrapper>(value_operand.as_real());
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
