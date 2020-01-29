#include <iostream>

#include "IrFunction.hh"
#include "ast/FunctionDecl.hh"

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

unsigned new_variable_id() {
    static unsigned variable_id = 0;
    return variable_id++;
}

Operand IrFunction::create_immediate_int(unsigned long value) const {
    OperandData data;
    data.immediate_int = value;

    Operand operand { OperandKind::IMMEDIATE_INT, data };

    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_immediate_string(std::string_view value) const {
    OperandData data;
    data.immediate_string = value;

    Operand operand { OperandKind::IMMEDIATE_STRING, data };

    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_immediate_real(double value) const {
    OperandData data;
    data.immediate_real = value;

    Operand operand { OperandKind::IMMEDIATE_REAL, data };

    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_label() const {
    OperandData data;
    data.label_id = new_label_id();

    Operand operand { OperandKind::LABEL, data };

    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_variable_from_id(unsigned id) const {
    OperandData data;
    data.variable_id = id;

    Operand operand { OperandKind::VARIABLE, data };

    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_variable() const {
    return create_variable_from_id(new_variable_id());
}

Operand IrFunction::get_variable(std::string_view identifier) const {
    auto it = m_varname_to_id.find(identifier);

    ASSERT(it != std::end(m_varname_to_id));

    return create_variable_from_id(it->second);
}

Operand IrFunction::create_function_from_id(unsigned function_id) const {
    OperandData data;
    data.function_id = function_id;

    Operand operand { OperandKind::FUNCTION, data };

    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_and_queue_label() {
    auto label = create_label();
    queue_label(label);
    return label;
}

void IrFunction::add_quad(OPCode op_code, Operand dest, Operand src_a,
                          Operand src_b) {
    auto quad = std::make_unique<Quad>(op_code, dest, src_a, src_b);
    bind_queued_labels(quad.get());
    m_quads.push_back(std::move(quad));
}

void IrFunction::queue_label(const Operand &label) {
    ASSERT(label.is_label());

    m_waiting_labels.push_back(label.data().label_id);
}

void IrFunction::bind_queued_labels(Quad *quad) {
    if (m_waiting_labels.empty()) return;

    for (auto label_id : m_waiting_labels) {
        bind_label(label_id, quad);
    }

    m_waiting_labels.clear();
}

void IrFunction::bind_label(const Operand &label, Quad *quad) {
    ASSERT(label.is_label());

    bind_label(label.data().label_id, quad);
}

void IrFunction::bind_label(unsigned label_id, Quad *quad) {
    ASSERT(m_labels.find(label_id) == std::end(m_labels));

    quad->set_label(label_id);

    m_labels.insert({ label_id, quad });
}

void IrFunction::bind_variable(const Operand &variable,
                               const std::string_view var_name) {
    ASSERT(variable.is_variable());

    m_varname_to_id.insert_or_assign(var_name, variable.data().variable_id);
    m_variable_ref.insert_or_assign(variable.data().variable_id, var_name);
}

void IrFunction::dump_quads() const {
    for (auto &quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}

std::string_view IrFunction::resolve_variable_name(unsigned variable_id) const {
    auto it = m_variable_ref.find(variable_id);

    if (it != std::end(m_variable_ref)) {
        return it->second;
    }

    std::ostringstream oss {};
    oss << "tmp#" << variable_id;
    return oss.str();
}

void IrFunction::__dump_variables() const {
    for (auto &x : m_variable_ref) {
        std::cout << x.first << "=" << x.second << std::endl;
    }
}