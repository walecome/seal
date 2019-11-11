#pragma once

#include <string>

#define ENUMERATE_OPCODES()       \
    __ENUMERATE_OPCODE(ADD)       \
    __ENUMERATE_OPCODE(SUB)       \
    __ENUMERATE_OPCODE(MULT)      \
    __ENUMERATE_OPCODE(DIV)       \
    __ENUMERATE_OPCODE(CMP_EQ)    \
    __ENUMERATE_OPCODE(CMP_GT)    \
    __ENUMERATE_OPCODE(CMP_LT)    \
    __ENUMERATE_OPCODE(CMP_GTEQ)  \
    __ENUMERATE_OPCODE(CMP_LTEQ)  \
    __ENUMERATE_OPCODE(CMP_NOTEQ) \
    __ENUMERATE_OPCODE(JMP)       \
    __ENUMERATE_OPCODE(JMP_Z)     \
    __ENUMERATE_OPCODE(JMP_NZ)    \
    __ENUMERATE_OPCODE(PUSH_ARG)  \
    __ENUMERATE_OPCODE(CALL)      \
    __ENUMERATE_OPCODE(RET)       \
    __ENUMERATE_OPCODE(MOVE)

enum class OPCode {
#undef __ENUMERATE_OPCODE
#define __ENUMERATE_OPCODE(opcode) opcode,
    ENUMERATE_OPCODES()
#undef __ENUMERATE_OPCODE
};

inline constexpr const char* opcode_to_string(OPCode op_code) {
    switch (op_code) {
#undef __ENUMERATE_OPCODE
#define __ENUMERATE_OPCODE(opcode) \
    case OPCode::opcode:           \
        return #opcode;
        ENUMERATE_OPCODES()
#undef __ENUMERATE_OPCODE
    }

    return "UNKNOWN";
}