#include "CTypeWrapper.hh"

#include <memory>
#include <string_view>
#include <type_traits>

#include "Constants.hh"
#include "common/Value.hh"

namespace vm {

namespace {

class CIntWrapper : public CTypeWrapper {
   public:
    explicit CIntWrapper(unsigned long value)
        : CTypeWrapper(ffi_type_uint64), m_value(value) {
    }

    void* get_value() override {
        return &m_value;
    }

   private:
    unsigned long m_value;
};

class CStringWrapper : public CTypeWrapper {
   public:
    explicit CStringWrapper(std::string value)
        : CTypeWrapper(ffi_type_pointer), m_value(std::move(value)) {
    }

    void* get_value() {
        return m_value.data();
    }

   private:
    std::string m_value;
};

class CRealWrapper : public CTypeWrapper {
   public:
    explicit CRealWrapper(double value)
        : CTypeWrapper(ffi_type_double), m_value(value) {
    }

    void* get_value() override {
        return &m_value;
    }

   private:
    double m_value;
};

}  // namespace

ptr_t<CTypeWrapper> CTypeWrapper::from(const Value& value) {
    if (value.is_integer()) {
        return std::make_unique<CIntWrapper>(value.as_integer().value());
    }
    if (value.is_real()) {
        return std::make_unique<CRealWrapper>(value.as_real().value());
    }
    if (value.is_string()) {
        return std::make_unique<CStringWrapper>(std::string(value.as_string().value()));
    }
    ASSERT_NOT_REACHED_MSG("Unsupported value type");
}

}  // namespace vm
