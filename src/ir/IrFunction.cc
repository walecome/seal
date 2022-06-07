#include <fmt/format.h>
#include <memory>
#include <utility>
#include <vector>

#include "IrFunction.hh"
#include "Operand.hh"
#include "ast/FunctionDecl.hh"

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

LabelOperand IrFunction::create_label() const {
    return LabelOperand { new_label_id() };
}

FunctionOperand IrFunction::create_function_from_id(
    unsigned function_id) const {
    return FunctionOperand { function_id };
}

LabelOperand IrFunction::create_and_queue_label() {
    auto label = create_label();
    queue_label(label);
    return label;
}

void IrFunction::add_quad(OPCode op_code, Operand dest, Operand src_a,
                          Operand src_b) {
    m_quads.push_back(std::make_unique<Quad>(op_code, dest, src_a, src_b));
    bind_queued_labels(m_quads.size() - 1);
}

void IrFunction::queue_label(const LabelOperand& label) {
    m_waiting_labels.push_back(label);
}

void IrFunction::bind_queued_labels(size_t quad_idx) {
    if (m_waiting_labels.empty()) return;

    for (auto label_id : m_waiting_labels) {
        bind_label(label_id, quad_idx);
    }

    m_waiting_labels.clear();
}

void IrFunction::bind_label(LabelOperand label, size_t quad_idx) {
    ASSERT(m_labels.find(label) == std::end(m_labels));

    m_quads.at(quad_idx)->add_label(label);

    m_labels.insert({ label, quad_idx });
}

void IrFunction::dump_quads() const {
    for (auto& quad : m_quads) {
        std::cout << quad->to_string() << std::endl;
    }
}

void IrFunction::finalize(ConstantPool::Entry entry) {
    ASSERT(m_quads.front()->opcode() == OPCode::ALLOC_REGS);
    auto quad = std::make_unique<Quad>(OPCode::ALLOC_REGS, Operand::empty(),
                                       Operand { entry }, Operand::empty());
    m_quads.front() = std::move(quad);
}

unsigned IrFunction::id() const {
    return declaration()->function_id();
}

const Quad& IrFunction::get_last_quad() const {
    ASSERT(!m_quads.empty());
    return *m_quads.back();
}

size_t IrFunction::quad_idx(const LabelOperand label) const {
    return m_labels.at(label);
}

void IrFunction::enter_block() {
    m_variables.push_back({});
}

void IrFunction::exit_block() {
    m_variables.pop_back();
}

Register IrFunction::create_register() {
    return allocate_register();
}

Register IrFunction::create_register_for_identifier(
    std::string_view identifier) {
    auto& current_variables = current_block_variables();
    ASSERT(current_variables.find(identifier) == current_variables.end());

    Register allocated_register = allocate_register();
    current_variables.insert({ identifier, allocated_register });
    return allocated_register;
}

std::optional<Register> IrFunction::find_register_for_identifier(
    std::string_view identifier) {
    for (auto it = m_variables.rbegin(); it != m_variables.rend(); ++it) {
        auto allocated_entry = it->find(identifier);
        if (allocated_entry != it->end()) {
            return allocated_entry->second;
        }
    }
    return std::nullopt;
}

Register IrFunction::allocate_register() {
    return Register(m_register_count++);
}

std::map<std::string_view, Register>& IrFunction::current_block_variables() {
    return m_variables.back();
}
