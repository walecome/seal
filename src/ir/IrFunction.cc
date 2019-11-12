#include <iostream>

#include "IrFunction.hh"
#include "ast/FunctionDecl.hh"

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

unsigned new_variable_id() {
    static unsigned label_id = 0;
    return label_id++;
}

Operand IrFunction::create_immediate(unsigned long value) {
    Operand operand {};
    operand.kind() = OperandKind::IMMEDIATE;
    operand.data().immediate_value = value;

    return operand;
}

Operand IrFunction::create_label() {
    Operand operand {};
    operand.kind() = OperandKind::LABEL;
    operand.data().variable_id = new_label_id();

    return operand;
}

Operand IrFunction::create_variable() {
    Operand operand {};
    operand.kind() = OperandKind::VARIABLE;
    operand.data().variable_id = new_variable_id();

    return operand;
}

void IrFunction::emplace_quad(OPCode op_code, Operand dest, Operand src_a,
                              Operand src_b) {
    m_quads.emplace_back(std::make_unique<Quad>(op_code, dest, src_a, src_b));
}

void IrFunction::dump_quads() const {
    for (auto &quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}