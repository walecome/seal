#include "common/Operand.hh"

Operand::Operand(BuiltinFunctionAddress addr)
    : m_type(Type::BuiltinAddress), m_data(addr) {
}

Operand::Operand(ConstantPool::Entry entry)
    : m_type(Type::ConstantEntry), m_data(entry) {
}

Operand::Operand(InstructionAddress addr)
    : m_type(Type::Address), m_data(addr) {
}

Operand::Operand(Register reg) : m_type(Type::Reg), m_data(reg) {
}

Operand::Operand() : m_type(Type::Empty), m_data(std::nullopt) {
}

Operand Operand::empty() {
    return Operand();
}

Operand::Data::Data(BuiltinFunctionAddress addr) : builtin_addr { addr } {
}

Operand::Data::Data(ConstantPool::Entry entry) : constant_entry { entry } {
}

Operand::Data::Data(InstructionAddress addr) : addr { addr } {
}

Operand::Data::Data(Register reg) : reg { reg } {
}

bool Operand::is_address() const {
    return m_type == Type::Address;
}

bool Operand::is_builtin() const {
    return m_type == Type::BuiltinAddress;
}

bool Operand::is_constant() const {
    return m_type == Type::ConstantEntry;
}

bool Operand::is_register() const {
    return m_type == Type::Reg;
}

bool Operand::is_used() const {
    return m_type != Type::Empty;
}

BuiltinFunctionAddress Operand::as_builtin() const {
  ASSERT(is_builtin());
  return m_data->builtin_addr;
}

ConstantPool::Entry Operand::as_constant() const {
  ASSERT(is_constant());
  return m_data->constant_entry;
}

InstructionAddress Operand::as_address() const {
  ASSERT(is_address());
  return m_data->addr;
}

Register Operand::as_register() const {
  ASSERT(is_register());
  return m_data->reg;
}

std::string Operand::to_debug_string() const {
    switch (m_type) {
        case Type::BuiltinAddress:
            return fmt::format("BuiltinFunctionAddress${}", m_data->builtin_addr.id());
        case Type::ConstantEntry:
            return fmt::format("ConstantPool::Entry${}", m_data->constant_entry.key);
        case Type::Empty:
            return "Empty";
        case Type::Address:
            return fmt::format("InstructionAddress${}", m_data->addr.unwrap());
        case Type::Reg:
            return fmt::format("Register${}", m_data->reg.index());
    }
    ASSERT_NOT_REACHED();
}

std::string Operand::to_string() const {
    switch (m_type) {
        case Type::BuiltinAddress:
            return fmt::format("$${}", m_data->builtin_addr.id());
        case Type::ConstantEntry:
            return fmt::format("C${}", m_data->constant_entry.key);
        case Type::Empty:
            return "_";
        case Type::Address:
            return fmt::format("${}", m_data->addr.unwrap());
        case Type::Reg:
            return fmt::format("r${}", m_data->reg.index());
    }
    ASSERT_NOT_REACHED();
}
