#include <fmt/format.h>

#include "IrFunction.hh"
#include "ast/FunctionDecl.hh"

unsigned new_basic_block_id() {
    static unsigned id = 0;
    return id++;
}

BasicBlock::BasicBlock() : m_id { new_basic_block_id() } {}

void BasicBlock::add_quad(Quad quad) { m_quads.push_back(quad); }

void BasicBlock::add_parent(BasicBlock *block) { m_parents.push_back(block); }

void BasicBlock::print_quads() const {
    bool first = true;
    for (auto &quad : m_quads) {
        if (first) {
            first = false;
            fmt::print("|{}|\n", quad.to_string(this));
        } else {
            fmt::print("|{}|\n", quad.to_string(nullptr));
        }
    }
}

std::string BasicBlock::name() const { return fmt::format("BB#{}", m_id); }

const std::vector<BasicBlock *> &BasicBlock::parents() const {
    return m_parents;
}

void BasicBlock::assign_variable(VariableOperand var,
                                 std::string_view identifier) {
    m_varname_to_id.insert_or_assign(identifier, var);
}

BasicBlock *IrFunction::new_basic_block(BasicBlock *parent) {
    m_basic_blocks.push_back(std::make_unique<BasicBlock>());
    if (parent) {
        current_block()->add_parent(parent);
    }

    return current_block();
}

unsigned BasicBlock::get_variable(std::string_view identifier) const {
    auto it = m_varname_to_id.find(identifier);
    ASSERT(it != m_varname_to_id.end());

    return it->second;
}

BasicBlock *IrFunction::new_basic_block(std::vector<BasicBlock *> &parents) {
    m_basic_blocks.push_back(std::make_unique<BasicBlock>());

    for (BasicBlock *parent : parents) {
        current_block()->add_parent(parent);
    }

    return current_block();
}

unsigned new_label_id() {
    static unsigned label_id = 0;
    return label_id++;
}

unsigned new_variable_id() {
    static unsigned variable_id = 0;
    return variable_id++;
}

template <class ValueType, class T = decltype(ValueType().value)>
ValueOperand create_value_operand(T value) {
    return ValueOperand { ValueType { value } };
}

Operand IrFunction::create_immediate_int(unsigned long value) const {
    Operand operand { OperandKind::IMMEDIATE_INT,
                      create_value_operand<IntOperand>(value) };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_immediate_string(std::string_view value) const {
    Operand operand { OperandKind::IMMEDIATE_STRING,
                      create_value_operand<StringOperand>(value) };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_immediate_real(double value) const {
    Operand operand { OperandKind::IMMEDIATE_REAL,
                      create_value_operand<RealOperand>(value) };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_label() const {
    Operand operand { OperandKind::LABEL, LabelOperand { new_label_id() } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_variable_from_id(unsigned id) const {
    Operand operand { OperandKind::VARIABLE, VariableOperand { id } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_variable() const {
    return create_variable_from_id(new_variable_id());
}

/*

                    a0
        a1                      a2
    a3      a4           a5         a6
        a7 = phi(a3, a4)    a8 = phi(a5, a6)
                    a9 = phi(a7, a8)
*/

Operand IrFunction::get_variable(std::string_view identifier) const {
    auto it = m_var_to_blocks.find(identifier);
    ASSERT(it != std::end(m_var_to_blocks));

    const std::vector<BasicBlock *> candidate_blocks = it->second;

    if (candidate_blocks.size() == 1) {
        return create_variable_from_id(
            candidate_blocks[0]->get_variable(identifier));
    }

    for (const BasicBlock *block : candidate_blocks) {
        // If the variable exists in the current block, use it
        if (block == current_block()) {
            return create_variable_from_id(block->get_variable(identifier));
        }
    }

    // FOR TESTING: Return from the latest basic block
    return create_variable_from_id(
        candidate_blocks.back()->get_variable(identifier));

    // TODO
    ASSERT_NOT_REACHED();
}

Operand IrFunction::create_function_from_id(unsigned function_id) const {
    Operand operand { OperandKind::FUNCTION, FunctionOperand { function_id } };
    operand.set_env(this);

    return operand;
}

Operand IrFunction::create_and_queue_label() {
    auto label = create_label();
    queue_label(label);
    return label;
}

void IrFunction::construct_quad(OPCode op_code, Operand dest, Operand src_a,
                                Operand src_b) {
    current_block()->add_quad({ Quad { op_code, dest, src_a, src_b } });

    // TODO: How?
    bind_queued_labels(m_basic_blocks.size() - 1);
}

// Add a label to waiting labels
void IrFunction::queue_label(const Operand &label) {
    ASSERT(label.is_label());
    m_waiting_labels.push_back(std::get<LabelOperand>(label.data()));
}

// Binds the queued labels to the given quad. Also clears the label queue.
void IrFunction::bind_queued_labels(size_t block_idx) {
    if (m_waiting_labels.empty()) return;

    for (auto label_id : m_waiting_labels) {
        bind_label(label_id, block_idx);
    }

    m_waiting_labels.clear();
}

void IrFunction::bind_label(LabelOperand label, size_t block_idx) {
    ASSERT(m_labels.find(label) == std::end(m_labels));

    m_basic_blocks.at(block_idx)->back().set_label(label);
    m_labels.insert({ label, block_idx });
}

// Bind a source code variable name to an IR variable
void IrFunction::bind_variable(const Operand &variable,
                               const std::string_view var_name) {
    ASSERT(variable.is_variable());

    VariableOperand var_raw = std::get<VariableOperand>(variable.data());
    current_block()->assign_variable(var_raw, var_name);

    if (!m_var_to_blocks.count(var_name)) {
        m_var_to_blocks.insert({ var_name, {} });
    }

    m_var_to_blocks.at(var_name).push_back(current_block());
    m_variable_ref.insert_or_assign(var_raw, var_name);
}

void IrFunction::print_blocks() const {
    std::string header = fmt::format("{:<12}{:<10}|{:>16}|{:>16}|{:>16}", "",
                                     "OPCODE", "DEST", "SRC_A", "SRC_B");

    int header_width = header.size();

    fmt::print("|{:-<{}}|\n", "", header_width);
    fmt::print("|{}|\n", header);

    for (auto &block : m_basic_blocks) {
        fmt::print("|{:-<{}}|\n", "", header_width);
        block->print_quads();
    }
    fmt::print("|{:-<{}}|\n", "", header_width);

    fmt::print("Basic block relations:\n");
    for (auto &block : m_basic_blocks) {
        fmt::print("Basic block {} has parents: ", block->name());
        for (auto child_block : block->parents()) {
            fmt::print("{}, ", child_block->name());
        }

        fmt::print("\n");
    }
}

// Return the name bound to variable_id if present, otherwise
// tmp#<variable_id>. This function is used for printing quads, so performance
// is not critical.
std::string_view IrFunction::resolve_variable_name(unsigned variable_id) const {
    auto it = m_variable_ref.find(variable_id);

    if (it != std::end(m_variable_ref)) {
        return it->second;
    }

    std::ostringstream oss {};
    oss << "tmp#" << variable_id;
    return oss.str();
}

unsigned IrFunction::id() const { return declaration()->function_id(); }

// Return the index of the quad bound to label
size_t IrFunction::quad_idx(const LabelOperand label) const {
    return m_labels.at(label);
}
