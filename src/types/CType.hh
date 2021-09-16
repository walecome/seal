#pragma once

#include <fmt/format.h>

#include "ffi.h"

#include "Constants.hh"
#include "Type.hh"

namespace ctype {

namespace {
    constexpr inline unsigned void_type_id   = 0;
    constexpr inline unsigned string_type_id = 1;
    constexpr inline unsigned int_type_id    = 2;
    constexpr inline unsigned real_type_id   = 3;
}

struct TypeInfo {
    const Type seal_type;
    const ffi_type c_type;
    const unsigned type_id;
};

static const TypeInfo void_info {
    Type(Primitive::VOID),
    ffi_type_void,
    void_type_id
};

static const TypeInfo string_info {
    Type(Primitive::STRING),
    ffi_type_pointer,
    string_type_id
};

static const TypeInfo int_info {
    Type(Primitive::INT),
    ffi_type_uint64,
    int_type_id
};

static const TypeInfo real_info {
    Type(Primitive::FLOAT),
    ffi_type_double,
    real_type_id,
};

static inline TypeInfo from_type_id(unsigned type_id) {
    switch (type_id) {
        case void_type_id:
            return void_info;

        case string_type_id:
            return string_info;

        case int_type_id:
            return int_info;

        case real_type_id:
            return real_info;
    }

    ASSERT_NOT_REACHED_MSG(fmt::format("Unexpected type_id: {}", type_id));
}

static inline TypeInfo from_seal_type(Type type) {
    switch (type.primitive()) {
        case Primitive::VOID:
            return void_info;

        case Primitive::STRING:
            return string_info;

        case Primitive::INT:
            return int_info;

        case Primitive::FLOAT:
            return real_info;

        default:
            ASSERT_NOT_REACHED_MSG(fmt::format("Unsupport from_seal_string on: {}", type.to_string()));
    }
}

}
