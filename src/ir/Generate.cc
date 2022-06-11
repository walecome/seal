#include <algorithm>
#include <iterator>

#include "Generate.hh"
#include "IrFunction.hh"
#include "Register.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/Block.hh"
#include "ast/BooleanLiteral.hh"
#include "ast/CompareExpression.hh"
#include "ast/CompilationUnit.hh"
#include "ast/EqualityExpression.hh"
#include "ast/Expression.hh"
#include "ast/For.hh"
#include "ast/FunctionCall.hh"
#include "ast/FunctionDecl.hh"
#include "ast/FunctionDeclC.hh"
#include "ast/FunctionDeclUser.hh"
#include "ast/IfStatement.hh"
#include "ast/IndexExpression.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Literal.hh"
#include "ast/RealLiteral.hh"
#include "ast/ReturnStatement.hh"
#include "ast/StringLiteral.hh"
#include "ast/UnaryExpression.hh"
#include "ast/VariableDecl.hh"
#include "ast/VariableExpression.hh"
#include "ast/While.hh"
#include "builtin/BuiltIn.hh"

#include "ir/IrProgram.hh"
#include "types/CType.hh"

#include "common/ConstantPool.hh"
#include "common/RelocatedQuad.hh"

#include "fmt/format.h"

Generate::Generate(const CompilationUnit *compilation_unit,
                   ConstantPool *constant_pool)
    : m_compilation_unit(compilation_unit), m_constant_pool(constant_pool) {
}

QuadCollection Generate::generate() {
    auto ir_program = std::make_unique<IrProgram>();

    m_compilation_unit->for_each_function([&](auto function_decl) {
        m_current_ir_function = std::make_unique<IrFunction>(function_decl);
        gen_function_decl(function_decl);

        env().finalize(get_constant_pool().add(
            Value::create_integer(env().register_count())));

        const FunctionDecl *decl = m_current_ir_function->declaration();
        m_quad_collection.function_to_quad.insert(
            { decl->function_id(), m_quad_collection.quads.size() });
        m_quad_collection.function_to_string.insert(
            { decl->function_id(), std::string(decl->identifier()) });

        m_current_ir_function->for_each_quad([&](Quad *quad) {
            for (unsigned label_id : quad->label_ids()) {
                m_quad_collection.label_to_quad.insert(
                    { label_id, m_quad_collection.quads.size() });
            }
            m_quad_collection.quads.push_back(*quad);
        });

        ir_program->add_function(m_current_ir_function);
    });

    m_quad_collection.main_function_id = ir_program->main_function_id();

    m_quad_collection.register_count = m_register_count;

    return m_quad_collection;
}

void Generate::gen_block(const Block *block) {
    env().enter_block();
    block->for_each_node([this](Node *node) {
        if (auto x = dynamic_cast<Statement *>(node))
            gen_statement(x);

        else if (auto x = dynamic_cast<FunctionDecl *>(node)) {
            ASSERT_NOT_REACHED();
        }

        else
            ASSERT_NOT_REACHED();
    });
    env().exit_block();
}

void Generate::gen_function_decl(const FunctionDecl *function_decl) {
    env().enter_block();
    // Temporary
    env().add_quad(OPCode::ALLOC_REGS, Operand::empty(), Operand::empty(),
                   Operand::empty());

    function_decl->parameter_list()->for_each_parameter([this](auto param) {
        Register reg =
            env().create_register_for_identifier(param->identifier());
        env().add_quad(OPCode::POP_ARG, Operand { reg }, Operand::empty(),
                       Operand::empty());
    });

    if (auto x = dynamic_cast<const FunctionDeclUser *>(function_decl)) {
        gen_user_function_decl(x);
    }

    // Need to insert a RET if there wasn't an explicit one.
    if (env().get_last_quad().opcode() != OPCode::RET) {
        env().add_quad(OPCode::RET, Operand::empty(), Operand::empty(),
                       Operand::empty());
    }
    env().exit_block();
}

void Generate::gen_user_function_decl(const FunctionDeclUser *decl) {
    gen_block(decl->body());
}

void Generate::gen_variable_decl(const VariableDecl *variable_decl) {
    if (variable_decl->value()) {
        auto value = gen_expression(variable_decl->value());
        auto dest = Operand { env().create_register_for_identifier(
            variable_decl->identifier()) };
        env().add_quad(OPCode::MOVE, dest, value, Operand::empty());
    }
}

void Generate::gen_statement(const Statement *statement) {
    if (auto ptr = dynamic_cast<const Block *>(statement)) {
        gen_block(ptr);
    } else if (auto ptr = dynamic_cast<const VariableDecl *>(statement)) {
        gen_variable_decl(ptr);
    } else if (auto ptr = dynamic_cast<const Expression *>(statement)) {
        gen_expression(ptr);
    } else if (auto ptr = dynamic_cast<const IfStatement *>(statement)) {
        gen_if_statement(ptr);
    } else if (auto ptr = dynamic_cast<const While *>(statement)) {
        gen_while(ptr);
    } else if (auto ptr = dynamic_cast<const ReturnStatement *>(statement)) {
        gen_return(ptr);
    } else if (auto ptr = dynamic_cast<const For *>(statement)) {
        gen_for(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

void Generate::gen_if_statement(const IfStatement *if_statement) {
    auto condition = gen_expression(if_statement->condition());

    auto else_label = Operand { env().create_label() };
    env().add_quad(OPCode::JMP_Z, else_label, condition, Operand::empty());

    gen_block(if_statement->if_body());

    if (if_statement->else_body()) {
        auto end_label = Operand { env().create_label() };
        env().add_quad(OPCode::JMP, end_label, Operand::empty(),
                       Operand::empty());
        env().queue_label(else_label.as_label());
        gen_block(if_statement->else_body());
        env().queue_label(end_label.as_label());
    } else {
        env().queue_label(else_label.as_label());
    }
}

void Generate::gen_while(const While *while_statement) {
    auto condition_label = Operand { env().create_and_queue_label() };
    auto condition = gen_expression(while_statement->condition());

    auto end_label = Operand { env().create_label() };
    env().add_quad(OPCode::JMP_Z, end_label, condition, Operand::empty());

    gen_block(while_statement->body());

    env().add_quad(OPCode::JMP, condition_label, Operand::empty(),
                   Operand::empty());
    env().queue_label(end_label.as_label());

    while_statement->body();
}

void Generate::gen_for(const For *for_statement) {
    gen_variable_decl(for_statement->initial_expression());

    auto condition_label = Operand { env().create_and_queue_label() };

    auto stop_condition = gen_expression(for_statement->stop_condition());

    auto end_label = Operand { env().create_label() };
    env().add_quad(OPCode::JMP_Z, end_label, stop_condition, Operand::empty());

    gen_block(for_statement->body());
    gen_expression(for_statement->per_iteration_expression());
    env().add_quad(OPCode::JMP, condition_label, Operand::empty(),
                   Operand::empty());

    env().queue_label(end_label.as_label());
}

void Generate::gen_return(const ReturnStatement *return_statement) {
    Operand source = gen_expression(return_statement->return_value());
    env().add_quad(OPCode::RET, Operand::empty(), source, Operand::empty());
}

Operand Generate::gen_expression(const Expression *expression) {
    if (auto ptr = dynamic_cast<const BinaryExpression *>(expression)) {
        return gen_binary_expression(ptr);
    } else if (auto ptr = dynamic_cast<const FunctionCall *>(expression)) {
        return gen_function_call(ptr);
    } else if (auto ptr = dynamic_cast<const IndexExpression *>(expression)) {
        return gen_index_expression(ptr);
    } else if (auto ptr = dynamic_cast<const Literal *>(expression)) {
        return Operand { create_literal(ptr) };
    } else if (auto ptr = dynamic_cast<const UnaryExpression *>(expression)) {
        return gen_unary_expression(ptr);
    } else if (auto ptr =
                   dynamic_cast<const VariableExpression *>(expression)) {
        return gen_variable_expression(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

Operand Generate::gen_function_call(const FunctionCall *func_call) {
    // Push arguments
    func_call->argument_list()->for_each_enumerated_argument(
        [&](auto arg, unsigned) {
            auto arg_operand = gen_expression(arg);
            env().add_quad(OPCode::PUSH_ARG, Operand::empty(), arg_operand,
                           Operand::empty());
        });

    bool is_builtin = BuiltIn::is_builtin(func_call->identifier());

    if (is_builtin) {
        unsigned function_id =
            BuiltIn::function_id_from_identifier(func_call->identifier());
        FunctionOperand func = env().create_function_from_id(function_id);
        Operand destination = Operand { env().create_register() };
        env().add_quad(OPCode::CALL, destination, Operand { func },
                       Operand::empty());
        return destination;
    }

    FunctionDecl *decl = func_call->declaration();

    Operand destination = decl->type().is_void()
                              ? Operand::empty()
                              : Operand { env().create_register() };

    if (auto *x = dynamic_cast<FunctionDeclC *>(decl)) {
        Operand lib = Operand(
            get_constant_pool().add(Value::create_string(x->lib_name())));
        Operand func = Operand(
            get_constant_pool().add(Value::create_string(x->identifier())));

        unsigned type_id = ctype::from_seal_type(x->type()).type_id;
        env().add_quad(
            OPCode::SET_RET_TYPE, Operand::empty(),
            Operand(get_constant_pool().add(Value::create_integer(type_id))),
            Operand::empty());
        env().add_quad(OPCode::CALL_C, destination, Operand { lib },
                       Operand { func });
    } else {
        auto function_id = func_call->declaration()->function_id();
        FunctionOperand func = env().create_function_from_id(function_id);
        env().add_quad(OPCode::CALL, destination, Operand { func },
                       Operand::empty());
    }

    return destination;
}

Operand Generate::gen_assign_expression(const AssignExpression *assign_expr) {
    ASSERT(assign_expr->op()->symbol() == OperatorSym::ASSIGN);

    Operand right = gen_expression(assign_expr->right());

    VariableExpression *var =
        dynamic_cast<VariableExpression *>(assign_expr->left());
    if (var) {
        Operand left = gen_variable_expression(var);
        env().add_quad(OPCode::MOVE, Operand { left.as_register() }, right,
                       Operand::empty());
        return left;
    }

    // TODO: Not sure if this should be done here or somewhere else. The
    // IndexExpression in not evaluated not, which is kind of counter
    // intuitive...
    IndexExpression *index_expr =
        dynamic_cast<IndexExpression *>(assign_expr->left());

    if (index_expr) {
        Register indexed =
            gen_expression(index_expr->indexed_expression()).as_register();
        Operand index = gen_expression(index_expr->index());
        env().add_quad(OPCode::INDEX_ASSIGN, Operand { indexed }, index, right);
        return right;
    }

    ASSERT_NOT_REACHED();
}

Operand Generate::gen_equality_expression(
    const EqualityExpression *equality_expr) {
    auto result = env().create_register();

    auto left = gen_expression(equality_expr->left());
    auto right = gen_expression(equality_expr->right());

    switch (equality_expr->op()->symbol()) {
        case OperatorSym::EQ:
            env().add_quad(OPCode::CMP_EQ, Operand { result }, left, right);
            break;

        case OperatorSym::NOT_EQ:
            env().add_quad(OPCode::CMP_NOTEQ, Operand { result }, left, right);
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return Operand { result };
}

Operand Generate::gen_compare_expression(
    const CompareExpression *compare_expr) {
    auto result = env().create_register();

    auto result_dest = Operand { result };

    auto left = gen_expression(compare_expr->left());
    auto right = gen_expression(compare_expr->right());

    switch (compare_expr->op()->symbol()) {
        case OperatorSym::LT:
            env().add_quad(OPCode::CMP_LT, result_dest, left, right);
            break;

        case OperatorSym::LTEQ:
            env().add_quad(OPCode::CMP_LTEQ, result_dest, left, right);
            break;

        case OperatorSym::GT:
            env().add_quad(OPCode::CMP_GT, result_dest, left, right);
            break;

        case OperatorSym::GTEQ:
            env().add_quad(OPCode::CMP_GTEQ, result_dest, left, right);
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    return Operand { result };
}

Operand Generate::gen_binary_expression(const BinaryExpression *bin_expr) {
    if (auto ptr = dynamic_cast<const AssignExpression *>(bin_expr)) {
        return gen_assign_expression(ptr);
    } else if (auto ptr = dynamic_cast<const CompareExpression *>(bin_expr)) {
        return gen_compare_expression(ptr);
    } else if (auto ptr = dynamic_cast<const EqualityExpression *>(bin_expr)) {
        return gen_equality_expression(ptr);
    }

    auto right = gen_expression(bin_expr->right());
    auto left = gen_expression(bin_expr->left());

    auto result = env().create_register();

    auto result_dest = Operand { result };

    switch (bin_expr->op()->symbol()) {
        case OperatorSym::PLUS:
            env().add_quad(OPCode::ADD, result_dest, left, right);
            break;

        case OperatorSym::MINUS:
            env().add_quad(OPCode::SUB, result_dest, left, right);
            break;

        case OperatorSym::MULT:
            env().add_quad(OPCode::MULT, result_dest, left, right);
            break;

        case OperatorSym::DIV:
            env().add_quad(OPCode::DIV, result_dest, left, right);
            break;

        case OperatorSym::AND:
            env().add_quad(OPCode::AND, result_dest, left, right);
            break;

        case OperatorSym::OR:
            env().add_quad(OPCode::OR, result_dest, left, right);
            break;

        // @TODO: Handle
        case OperatorSym::MODULO:
        case OperatorSym::INC:
        case OperatorSym::DEC:

        case OperatorSym::NOT_EQ:
        case OperatorSym::LTEQ:
        case OperatorSym::LT:
        case OperatorSym::GTEQ:
        case OperatorSym::GT:
        case OperatorSym::EQ:
        case OperatorSym::ASSIGN:
        default:
            ASSERT_NOT_REACHED();
    }

    return Operand { result };
}

Operand Generate::gen_index_expression(const IndexExpression *expr) {
    Operand indexed = gen_expression(expr->indexed_expression());
    Operand index = gen_expression(expr->index());
    Register result = env().create_register();

    env().add_quad(OPCode::INDEX_MOVE, Operand { result }, indexed, index);

    return Operand { result };
}

Operand Generate::create_literal(const Literal *literal) {
    if (auto ptr = dynamic_cast<const ArrayLiteral *>(literal)) {
        return create_array_literal(ptr);
    } else if (auto ptr = dynamic_cast<const BooleanLiteral *>(literal)) {
        return create_boolean_literal(ptr);
    } else if (auto ptr = dynamic_cast<const RealLiteral *>(literal)) {
        return create_float_literal(ptr);
    } else if (auto ptr = dynamic_cast<const IntegerLiteral *>(literal)) {
        return create_integer_literal(ptr);
    } else if (auto ptr = dynamic_cast<const StringLiteral *>(literal)) {
        return create_string_literal(ptr);
    } else {
        ASSERT_NOT_REACHED();
    }
}

Operand Generate::gen_unary_expression(const UnaryExpression *unary_expr) {
    auto target_operand = gen_expression(unary_expr->expression());
    auto sym = unary_expr->op()->symbol();

    if (sym == OperatorSym::MINUS) {
        auto result = env().create_register();
        auto zero = Operand(get_constant_pool().add(Value::create_integer(0)));
        env().add_quad(OPCode::SUB, Operand(result), zero, target_operand);
        return Operand { result };
    }

    VariableExpression *var_expr =
        dynamic_cast<VariableExpression *>(unary_expr->expression());

    Register result = var_expr ? gen_variable_expression(var_expr).as_register()
                               : env().create_register();

    auto one = Operand(get_constant_pool().add(Value::create_integer(1)));

    if (sym == OperatorSym::INC) {
        env().add_quad(OPCode::ADD, Operand { result }, target_operand, one);
    } else if (sym == OperatorSym::DEC) {
        env().add_quad(OPCode::SUB, Operand { result }, target_operand, one);
    } else {
        ASSERT_NOT_REACHED();
    }

    return Operand { result };
}

Operand Generate::gen_variable_expression(const VariableExpression *var_expr) {
    std::optional<Register> maybe_reg =
        env().find_register_for_identifier(var_expr->identifier());
    ASSERT(maybe_reg.has_value());
    return Operand { maybe_reg.value() };
}

Operand Generate::create_integer_literal(
    const IntegerLiteral *integer_literal) {
    ConstantPool::Entry entry = get_constant_pool().add(
        Value::create_integer(integer_literal->value()));
    return Operand(entry);
}

Operand Generate::create_array_literal(const ArrayLiteral *array) {
    ASSERT_NOT_REACHED_MSG("Generate::create_array_literal not implemented");
    // std::vector<ConstantPool::Entry> values;

    // array->for_each_element([&](auto *element) {
    //     PoolEntry element_entry = gen_expression(element).as_value_entry();
    //     values.push_back(element_entry);
    // });
    // return Operand(get_constant_pool().create_vector(values));
}

Operand Generate::create_boolean_literal(
    const BooleanLiteral *boolean_literal) {
    Operand(get_constant_pool().add(
        Value::create_boolean(boolean_literal->value())));
}

Operand Generate::create_float_literal(const RealLiteral *real_literal) {
    return Operand(
        get_constant_pool().add(Value::create_real(real_literal->value())));
}

Operand Generate::create_string_literal(const StringLiteral *string_literal) {
    return Operand(
        get_constant_pool().add(Value::create_string(string_literal->value())));
}

ConstantPool &Generate::get_constant_pool() {
    return *m_constant_pool;
}

std::vector<RelocatedQuad> Generate::relocate_quads(
    const std::vector<Quad> &quads) {
    std::vector<RelocatedQuad> relocated_quads {};
    relocated_quads.reserve(quads.size());
    std::transform(quads.begin(), quads.end(),
                   std::back_inserter(relocated_quads),
                   [this](const Quad &quad) { return relocate_quad(quad); });
    return relocated_quads;
}

namespace {
RelocatedQuad::Operand convert_operand_without_relocation(
    const Operand &operand) {
    if (!operand.is_used()) {
        return RelocatedQuad::Operand::empty();
    }
    if (operand.is_constant_entry()) {
        return RelocatedQuad::Operand(operand.as_constant_entry());
    }
    if (operand.is_register()) {
        return RelocatedQuad::Operand(operand.as_register());
    }
    ASSERT_NOT_REACHED();
}
}  // namespace

RelocatedQuad Generate::relocate_quad(const Quad &quad) {
    if (!quad.needs_relocation()) {
        return RelocatedQuad(quad.opcode(),
                             convert_operand_without_relocation(quad.dest()),
                             convert_operand_without_relocation(quad.src_a()),
                             convert_operand_without_relocation(quad.src_b()));
    }
}
